#include "RaceCompatibility.h"

namespace Mus {
    bool RaceCompatibility::RaceController() 
    { 
        IsPlayerElin.store(isPlayerRaceTeraElinHumanoidVampireLord());

        if (!IsPlayerElin.load()) return false;

        logger::trace("RaceCompatibility Loading...");
        return ResolveCompatibleHairParts();
    }

    bool RaceCompatibility::GetRuntimeData() 
    {
        logger::trace("Building RuntimeData...");

        RunTimeHeadPartFormList = static_cast<RE::BGSListForm*>(RE::TESForm::LookupByID(HeadPartFormList));
        if (!RunTimeHeadPartFormList) 
        {
            logger::error("Can't Find HeadPartFormList");
            IsProblem = true;
            return false;
        }

        RE::TESForm* RunTimeTeraElinRaceForm = GetFormByID(TeraElinRaceFormID, TeraElinRaceESP);
        RE::TESForm* RunTimeTeraElinRaceVampForm = GetFormByID(TeraElinRaceVampFormID, TeraElinRaceESP);
        if (!RunTimeTeraElinRaceForm || !RunTimeTeraElinRaceVampForm)
        {
            logger::error("Can't Find Tera Elin Race ID");
            IsProblem = true;
            return false;
        }
        RunTimeTeraElinRaceFormID = RunTimeTeraElinRaceForm->formID;
        RunTimeTeraElinRaceVampFormID = RunTimeTeraElinRaceVampForm->formID;

        RE::TESForm* RunTimeTeraElinRaceFormList = GetFormByID(TeraElinRaceFormListID, TeraElinRaceESP);
        if (!RunTimeTeraElinRaceFormList)
        {
            logger::error("Can't Find Tera Elin Race Form List");
            IsProblem = true;
            return false;
        }
        RunTimeTeraElinRaceFormListID = RunTimeTeraElinRaceFormList->formID;

        RE::TESForm* RunTimePlayableForm = GetFormByID(PlayableFormList, RaceCompatibilityESP);
        if (!RunTimePlayableForm)
        {
            logger::error("Can't Find RaceCompatibility FormList");
            IsProblem = true;
            return false;
        }
        RunTimePlayableFormList = static_cast<RE::BGSListForm*>(RunTimePlayableForm);

        RE::TESForm* RunTimePlayableVampForm = GetFormByID(PlayableVampFormList, RaceCompatibilityESP);
        if (!RunTimePlayableVampForm)
        {
            logger::error("Can't Find RaceCompatibility VampireFormList");
            IsProblem = true;
            return false;
        }
        RunTimePlayableVampFormList = static_cast<RE::BGSListForm*>(RunTimePlayableVampForm);

        logger::trace("Building RuntimeData finished");

        return true;
    }

    bool RaceCompatibility::isPlayerRaceTeraElin()
    {
        if (IsProblem)
            return false;

        RE::PlayerCharacter* P = RE::PlayerCharacter::GetSingleton();

        if (!P)
            return false;

        if (P->GetActorBase()->GetRace()->formID == RunTimeTeraElinRaceFormID)
        {
            logger::info("Detected that Player Race is TeraElinRace");
            return true;
        }
        else if (P->GetActorBase()->GetRace()->formID == RunTimeTeraElinRaceVampFormID)
        {
            logger::info("Detected that Player Race is TeraElinRace Vampire");
            return true;
        }
        return false;
    }

    bool RaceCompatibility::isPlayerRaceTeraElinHumanoidVampireLord()
    {
        if (IsProblem)
            return false;

        if (!IsThereTheESP(HumanoidVampireLordESP) && !IsThereTheESP(NoMoreUglyVampireLordESP) && !IsThereTheESP(NoMoreUglyVampireLord2RVESP))
            return isPlayerRaceTeraElin();

        RE::PlayerCharacter* P = RE::PlayerCharacter::GetSingleton();

        if (!P)
            return false;

        if (IsPlayerElin.load() && P->GetActorBase()->GetRace()->formID == VampireLordRaceID)
        {
            logger::info("Detected that Player Race is TeraElinRace but now Vampire lord");
            return true;
        }

        return isPlayerRaceTeraElin();
    }

    bool RaceCompatibility::CreateHeadPartFormList()
    {
        logger::trace("Creating NewHeadPartFormList...");

        if (!RunTimeHeadPartFormList) 
            return false;

        if (!RunTimeHeadPartFormList->formType.all(RE::FormType::FormList)) 
            return false;

        if (!(CompatibleHeadPartRaces = static_cast<RE::BGSListForm*>(RunTimeHeadPartFormList->CreateDuplicateForm(true, (void*)CompatibleHeadPartRaces))))
            return false;

        CompatibleHeadPartRaces->SetFormEditorID("CompatibleHeadPartRaces");

        logger::trace("Create NewHeadPartFormList finished");
        return true;
    }

    bool RaceCompatibility::SetNonePlayable(RE::BGSHeadPart* headpart)
    {
        if (!headpart || !HeadPartExplorer::GetSingleton().IsPlayerble(headpart->flags))
            return false;

        auto& flag = headpart->flags;
        flag -= RE::BGSHeadPart::Flag::kPlayable;
        return true;
    }

    void RaceCompatibility::GetVanillaFormList()
    {
        auto& VHeadPartFormList = RunTimeHeadPartFormList->forms;

        for (RE::BSTArrayBase::size_type index = 0; index < VHeadPartFormList.size(); index++)
        {
            if (!VHeadPartFormList[index])
                continue;

            if (!VHeadPartFormList[index]->Is(RE::FormType::Race))
                continue;

            if (VHeadPartFormList[index]->formID == RunTimeTeraElinRaceFormID || VHeadPartFormList[index]->formID == RunTimeTeraElinRaceVampFormID)
                continue;

            if (IsBeastRace(VHeadPartFormList[index]->formID))
                continue;

            auto found = std::find(VanillaHeadPartFormList.begin(), VanillaHeadPartFormList.end(), VHeadPartFormList[index]->formID);

            if (found == VanillaHeadPartFormList.end())
            {
                VanillaHeadPartFormList.emplace_back(VHeadPartFormList[index]->formID);
                logger::debug("Found HeadPartRace {:x} {}", VHeadPartFormList[index]->formID, VHeadPartFormList[index]->GetName());
            }
        }

        auto& HeadPartFormList = RunTimePlayableFormList->forms;

        for (RE::BSTArrayBase::size_type index = 0; index < HeadPartFormList.size(); index++)
        {
            if (!HeadPartFormList[index])
                continue;

            if (!HeadPartFormList[index]->Is(RE::FormType::Race))
                continue;

            if (HeadPartFormList[index]->formID == RunTimeTeraElinRaceFormID || HeadPartFormList[index]->formID == RunTimeTeraElinRaceVampFormID)
                continue;

            if (IsBeastRace(HeadPartFormList[index]->formID))
                continue;

            auto found = std::find(VanillaHeadPartFormList.begin(), VanillaHeadPartFormList.end(), HeadPartFormList[index]->formID);

            if (found == VanillaHeadPartFormList.end())
            {
                VanillaHeadPartFormList.emplace_back(HeadPartFormList[index]->formID);
                logger::debug("Found HeadPartRace {:x} {}", HeadPartFormList[index]->formID, HeadPartFormList[index]->GetName());
            }
        }

        auto& HeadPartVampFormList = RunTimePlayableVampFormList->forms;

        for (RE::BSTArrayBase::size_type index = 0; index < HeadPartVampFormList.size(); index++)
        {
            if (!HeadPartVampFormList[index])
                continue;

            if (!HeadPartVampFormList[index]->Is(RE::FormType::Race))
                continue;

            if (HeadPartVampFormList[index]->formID == RunTimeTeraElinRaceFormID || HeadPartVampFormList[index]->formID == RunTimeTeraElinRaceVampFormID)
                continue;

            if (IsBeastRace(HeadPartVampFormList[index]->formID))
                continue;

            auto found = std::find(VanillaHeadPartFormList.begin(), VanillaHeadPartFormList.end(), HeadPartVampFormList[index]->formID);

            if (found == VanillaHeadPartFormList.end())
            {
                VanillaHeadPartFormList.emplace_back(HeadPartVampFormList[index]->formID);
                logger::debug("Found HeadPartRace {:x} {}", HeadPartVampFormList[index]->formID, HeadPartVampFormList[index]->GetName());
            }
        }
    }

    void RaceCompatibility::AddHeadPartRacesForm(RE::BGSListForm* formlist, bool isAddElin)
    {
        for (std::size_t index = 0; index < VanillaHeadPartFormList.size(); index++)
        {
            if (!VanillaHeadPartFormList[index])
                continue;

            RE::TESForm* TheForm = RE::TESForm::LookupByID(VanillaHeadPartFormList[index]);

            if (!TheForm || formlist->HasForm(TheForm))
                continue;

            formlist->AddForm(TheForm);

            logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), TheForm->formID, TheForm->GetName());
        }

        if (isAddElin)
        {
            RE::TESForm* ElinForm = RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID);
            RE::TESForm* ElinVampForm = RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID);
            if (ElinForm && !formlist->HasForm(ElinForm))
            {
                formlist->AddForm(ElinForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), ElinForm->formID, ElinForm->GetName());
            }
            if (ElinVampForm && !formlist->HasForm(ElinVampForm))
            {
                formlist->AddForm(ElinVampForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), ElinVampForm->formID, ElinVampForm->GetName());
            }
        }
    }

    void RaceCompatibility::AddHeadPartBeastsRacesForm(RE::BGSListForm* formlist, bool isArgonian)
    {
        if (!isArgonian)
        {
            RE::TESForm* TheForm = RE::TESForm::LookupByID(HeadPartExplorer::GetSingleton().Khajiit1);

            if (TheForm && !formlist->HasForm(TheForm))
            {
                formlist->AddForm(TheForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), TheForm->formID, TheForm->GetName());
            }

            TheForm = RE::TESForm::LookupByID(HeadPartExplorer::GetSingleton().Khajiit2);

            if (TheForm && !formlist->HasForm(TheForm))
            {
                formlist->AddForm(TheForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), TheForm->formID, TheForm->GetName());
            }
        }
        else
        {
            RE::TESForm* TheForm = RE::TESForm::LookupByID(HeadPartExplorer::GetSingleton().Argonian1);

            if (TheForm && !formlist->HasForm(TheForm))
            {
                formlist->AddForm(TheForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), TheForm->formID, TheForm->GetName());
            }

            TheForm = RE::TESForm::LookupByID(HeadPartExplorer::GetSingleton().Argonian2);

            if (TheForm && !formlist->HasForm(TheForm))
            {
                formlist->AddForm(TheForm);
                logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), TheForm->formID, TheForm->GetName());
            }
        }
    }
    
    void RaceCompatibility::AddHeadPartVampireLordForm(RE::BGSListForm* formlist)
    {
        if (!IsThereTheESP(HumanoidVampireLordESP) && !IsThereTheESP(NoMoreUglyVampireLordESP) && !IsThereTheESP(NoMoreUglyVampireLord2RVESP))
            return;

        RE::TESForm* VampireLordForm = RE::TESForm::LookupByID(VampireLordRaceID);

        if (VampireLordForm)
        {
            formlist->AddForm(VampireLordForm);
            logger::debug("Added HeadPartRace on {} FormList {:x} {}", formlist->GetFormEditorID(), VampireLordForm->formID, VampireLordForm->GetName());
        }
    }

    bool RaceCompatibility::RemoveHeadPartElinRacesForm()
    {
        if (IsProblem)
            return false;

        if (!Config::GetSingleton().GetBeforeSaveCompatible() || !Config::GetSingleton().GetRaceController() || !IsPlayerElin.load())
            return false;

        auto& HPE = HeadPartExplorer::GetSingleton();

        logger::info("Try to cleanup save script trashes... {}", HPE.OtherHeadPartFormMap.size());

        GetVanillaFormList();

        for (auto& h : HPE.OtherHeadPartFormMap)
        {
            if (!h.second || !h.second->validRaces)
                continue;

            if (std::find(doneHeadpartFormList.begin(), doneHeadpartFormList.end(), h.second->validRaces->formID) != doneHeadpartFormList.end())
                continue;

            auto& list = h.second->validRaces;
            bool isRemoved = false;
            if (list)
            {
                list->ClearData();
                AddHeadPartRacesForm(list, false);
                //AddHeadPartVampireLordForm(list);
                isRemoved = true;
            }

            if (isRemoved)
            {
                doneHeadpartFormList.emplace_back(list->formID);
                logger::info("Reset {:x} {} formlist", list->formID, list->GetFormEditorID());
            }
        }

        logger::info("Try to cleanup save script trashes... {}", HPE.KhajiitRacesHeadPartFormMap.size());

        for (auto& h : HPE.KhajiitRacesHeadPartFormMap)
        {
            if (!h.second || !h.second->validRaces)
                continue;

            if (std::find(doneHeadpartFormList.begin(), doneHeadpartFormList.end(), h.second->validRaces->formID) != doneHeadpartFormList.end())
                continue;

            auto& list = h.second->validRaces;
            bool isRemoved = false;
            if (list)
            {
                list->ClearData();
                AddHeadPartBeastsRacesForm(list, false);
                //AddHeadPartVampireLordForm(list);
                isRemoved = true;
            }

            if (isRemoved)
            {
                doneHeadpartFormList.emplace_back(list->formID);
                logger::info("Reset {:x} {} formlist", list->formID, list->GetFormEditorID());
            }
        }

        logger::info("Try to cleanup save script trashes... {}", HPE.ArgonianRacesHeadPartFormMap.size());

        for (auto& h : HPE.ArgonianRacesHeadPartFormMap)
        {
            if (!h.second || !h.second->validRaces)
                continue;

            if (std::find(doneHeadpartFormList.begin(), doneHeadpartFormList.end(), h.second->validRaces->formID) != doneHeadpartFormList.end())
                continue;

            auto& list = h.second->validRaces;
            bool isRemoved = false;
            if (list)
            {
                list->ClearData();
                AddHeadPartBeastsRacesForm(list, true);
                //AddHeadPartVampireLordForm(list);
                isRemoved = true;
            }

            if (isRemoved)
            {
                doneHeadpartFormList.emplace_back(list->formID);
                logger::info("Reset {:x} {} formlist", list->formID, list->GetFormEditorID());
            }
        }

        return true;
    }

    bool RaceCompatibility::InitFormList()
    {
        if (IsProblem)
            return false;

        auto& HPE = HeadPartExplorer::GetSingleton();
        if (!HPE.InitFormList())
        {
            logger::error("Can't Initial headpart form map");
            return false;
        }

        isInitFormList = true;
        return true;
    }

    bool RaceCompatibility::ResolveCompatibleHairParts() 
    {
        logger::trace("Try resolve Compatible issues in HeadParts...");
        auto& HPE = HeadPartExplorer::GetSingleton();

        if (!CompatibleHeadPartRaces)
        {
            if (!CreateHeadPartFormList())
            {
                logger::error("Can't Create New HeadPart ValidRaces List");
                return false;
            }
        }

        GetVanillaFormList();
        AddHeadPartRacesForm(CompatibleHeadPartRaces, true);
        AddHeadPartBeastsRacesForm(CompatibleHeadPartRaces, false);
        AddHeadPartBeastsRacesForm(CompatibleHeadPartRaces, true);
        AddHeadPartVampireLordForm(CompatibleHeadPartRaces);

        logger::info("Try to compatible work on hair parts... {}", HPE.HeadPartFormMap.size());
        concurrency::parallel_for_each(HPE.HeadPartFormMap.begin(), HPE.HeadPartFormMap.end(), [&](auto& HP) { 
            if (HP.second)
            {
                if (HP.second->validRaces)
                {
                    HP.second->validRaces = CompatibleHeadPartRaces;
                    logger::trace("Edit {}headpart for race compatible done : {} {} {:x}", (HP.second->IsExtraPart() ? "extra " : ""), GetModNameByID(HP.second->formID), HP.second->formEditorID.c_str(), HP.second->formID);
                }
            }
        });

        logger::info("Try to set to non playable on head parts with incorrect validraces list... {}", HPE.NoneRacesHeadPartFormMap.size());
        concurrency::parallel_for_each(HPE.NoneRacesHeadPartFormMap.begin(), HPE.NoneRacesHeadPartFormMap.end(), [&](auto& HP) {
            if (HP.second)
            {
                if (SetNonePlayable(HP.second))
                    logger::trace("Set {}headpart to none Playable done : {} {} {:x}", (HP.second->IsExtraPart() ? "extra " : ""), GetModNameByID(HP.second->formID), HP.second->formEditorID.c_str(), HP.second->formID);
            }
        });

        RE::TESForm* ElinRaceListForm = RE::TESForm::LookupByID(RunTimeTeraElinRaceFormListID);
        if (ElinRaceListForm)
        {
            RE::BGSListForm* ElinRaceList = static_cast<RE::BGSListForm*>(ElinRaceListForm);
            if (ElinRaceList)
            {
                AddHeadPartVampireLordForm(ElinRaceList);
                logger::info("VampireLord compatible patch for elin race done");
            }
        }

        logger::trace("Try resolve Compatible issues in HeadParts finished");
        return true;
    }

    void RaceCompatibility::LoadRaceCompatibility()
    {
        if (IsProblem)
            return;

        if (!Config::GetSingleton().GetRaceController() || !IsValidTeraElinRace)
        {
            logger::info("Disabled Race Compatibility Controller");
            return;
        }

        logger::trace("RaceController Loading...");
        if (RaceController()) 
            logger::info("RaceController finished");
        else
            logger::error("Can't load the RaceController");
        return;
    }

    //sacrosanct
    void RaceCompatibility::SolveCompatibleVampireSacrosanct()
    {
        if (!IsValidTeraElinRace)
            return;

        if (!IsThereTheESP(SacrosanctESP))
            return;

        RE::TESForm* SCS_RacesForm = GetFormByID(SCS_RacesFormID, SacrosanctESP);
        RE::TESForm* SCS_RacesVampForm = GetFormByID(SCS_RacesVampireFormID, SacrosanctESP);

        if (!SCS_RacesForm || !SCS_RacesVampForm)
            return;

        logger::info("Try compatible patch for {}...", SacrosanctESP);

        RE::BGSListForm* SCS_RacesFormlist = static_cast<RE::BGSListForm*>(SCS_RacesForm);
        RE::BGSListForm* SCS_RacesVampFormlist = static_cast<RE::BGSListForm*>(SCS_RacesVampForm);

        if (!SCS_RacesFormlist || !SCS_RacesVampFormlist)
            return;

        SCS_RacesFormlist->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID));
        SCS_RacesVampFormlist->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID));

        logger::info("Compatible patch for {} done", SacrosanctESP);
    }

    //vanilla
    void RaceCompatibility::SolveCompatibleVampireVanilla()
    {
        if (!RunTimePlayableFormList || !RunTimePlayableVampFormList)
            return;

        RunTimePlayableFormList->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID));
        RunTimePlayableVampFormList->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID));
    }

    //HumanoidVampireLord (HNV/NoMoreUgly)
    void RaceCompatibility::SolveCompatibleVampireLord()
    {
        if (!Config::GetSingleton().GetCompatibleHumanoidVampireLord())
            return;

        if (!isPlayerRaceTeraElinHumanoidVampireLord())
            return;

        std::string_view modname;
        if (IsThereTheESP(HumanoidVampireLordESP))
            modname = HumanoidVampireLordESP;
        else if (IsThereTheESP(NoMoreUglyVampireLordESP))
            modname = NoMoreUglyVampireLordESP;
        else if (IsThereTheESP(NoMoreUglyVampireLord2RVESP))
            modname = NoMoreUglyVampireLord2RVESP;
        else
            return;

        logger::info("Try compatible patch for Vampire Lord of {}...", modname);

        RE::TESRace* VampLordRace = GetRace(VampireLordRaceID);
        if (!VampLordRace)
            return;

        RE::TESRace* ElinRace = GetElinRace(false);
        if (!ElinRace)
            return;

        RE::TESRace* DefaultRace = GetRace(DefaultRaceID);
        if (!DefaultRace)
            return;

        VampireLordVanilla.skin = VampLordRace->skin;
        VampLordRace->skin = ElinRace->skin;

        VampireLordVanilla.height[RE::SEXES::kMale] = VampLordRace->data.height[RE::SEXES::kMale];
        VampireLordVanilla.height[RE::SEXES::kFemale] = VampLordRace->data.height[RE::SEXES::kFemale];
        VampLordRace->data.height[RE::SEXES::kMale] = ElinRace->data.height[RE::SEXES::kMale];
        VampLordRace->data.height[RE::SEXES::kFemale] = ElinRace->data.height[RE::SEXES::kFemale];

        if (VampireLordVanilla.kFaceGenHead = VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kFaceGenHead); !VampireLordVanilla.kFaceGenHead)
            VampLordRace->data.flags += RE::RACE_DATA::Flag::kFaceGenHead;
        if (VampireLordVanilla.kOverlayHeadPartList = VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kOverlayHeadPartList); VampireLordVanilla.kOverlayHeadPartList)
            VampLordRace->data.flags -= RE::RACE_DATA::Flag::kOverlayHeadPartList;
        if (VampireLordVanilla.kOverrideHeadPartList = VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kOverrideHeadPartList); VampireLordVanilla.kOverrideHeadPartList)
            VampLordRace->data.flags -= RE::RACE_DATA::Flag::kOverrideHeadPartList;

        nif::setBSFixedString(VampireLordVanilla.skeletonModels[RE::SEXES::kMale], VampLordRace->skeletonModels[RE::SEXES::kMale].GetModel());
        nif::setBSFixedString(VampireLordVanilla.skeletonModels[RE::SEXES::kFemale], VampLordRace->skeletonModels[RE::SEXES::kFemale].GetModel());
        VampLordRace->skeletonModels[RE::SEXES::kMale].SetModel(ElinRace->skeletonModels[RE::SEXES::kMale].GetModel());
        VampLordRace->skeletonModels[RE::SEXES::kFemale].SetModel(ElinRace->skeletonModels[RE::SEXES::kFemale].GetModel());

        VampLordRace->bipedObjectNameA[RE::BIPED_OBJECTS::kTail] = ElinRace->bipedObjectNameA[RE::BIPED_OBJECTS::kTail];

        VampireLordVanilla.faceRelatedData[RE::SEXES::kMale] = VampLordRace->faceRelatedData[RE::SEXES::kMale];
        VampireLordVanilla.faceRelatedData[RE::SEXES::kFemale] = VampLordRace->faceRelatedData[RE::SEXES::kFemale];
        VampLordRace->faceRelatedData[RE::SEXES::kMale] = ElinRace->faceRelatedData[RE::SEXES::kMale];
        VampLordRace->faceRelatedData[RE::SEXES::kFemale] = ElinRace->faceRelatedData[RE::SEXES::kFemale];

        VampireLordVanilla.morphRace = VampLordRace->morphRace;
        VampLordRace->morphRace = ElinRace;
        VampireLordVanilla.armorParentRace = VampLordRace->armorParentRace;
        VampLordRace->armorParentRace = DefaultRace;

        logger::info("Compatible patch for Vampire Lord of {} done", modname);
    }
    
    void RaceCompatibility::RevertCompatibleVampireLord()
    {
        if (!Config::GetSingleton().GetCompatibleHumanoidVampireLord())
            return;

        if (!isPlayerRaceTeraElinHumanoidVampireLord())
            return;

        std::string_view modname;
        if (IsThereTheESP(HumanoidVampireLordESP))
            modname = HumanoidVampireLordESP;
        else if (IsThereTheESP(NoMoreUglyVampireLordESP))
            modname = NoMoreUglyVampireLordESP;
        else if (IsThereTheESP(NoMoreUglyVampireLord2RVESP))
            modname = NoMoreUglyVampireLord2RVESP;
        else
            return;

        logger::info("Try revert compatible patch for Vampire Lord of {}...", modname);

        RE::TESRace* VampLordRace = GetRace(VampireLordRaceID);
        if (!VampLordRace)
            return;

        VampLordRace->skin = VampireLordVanilla.skin;

        VampLordRace->data.height[RE::SEXES::kMale] = VampireLordVanilla.height[RE::SEXES::kMale];
        VampLordRace->data.height[RE::SEXES::kFemale] = VampireLordVanilla.height[RE::SEXES::kFemale];

        if (VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kFaceGenHead) && !VampireLordVanilla.kFaceGenHead)
            VampLordRace->data.flags -= RE::RACE_DATA::Flag::kFaceGenHead;
        if (!VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kOverlayHeadPartList) && VampireLordVanilla.kOverlayHeadPartList)
            VampLordRace->data.flags += RE::RACE_DATA::Flag::kOverlayHeadPartList;
        if (!VampLordRace->data.flags.all(RE::RACE_DATA::Flag::kOverrideHeadPartList) && VampireLordVanilla.kOverrideHeadPartList)
            VampLordRace->data.flags += RE::RACE_DATA::Flag::kOverrideHeadPartList;

        VampLordRace->skeletonModels[RE::SEXES::kMale].SetModel(VampireLordVanilla.skeletonModels[RE::SEXES::kMale].c_str());
        VampLordRace->skeletonModels[RE::SEXES::kFemale].SetModel(VampireLordVanilla.skeletonModels[RE::SEXES::kFemale].c_str());
        //VampLordRace->bipedObjectNameA[RE::BIPED_OBJECTS::kTail] = ElinRace->bipedObjectNameA[RE::BIPED_OBJECTS::kTail];
        VampLordRace->faceRelatedData[RE::SEXES::kMale] = VampireLordVanilla.faceRelatedData[RE::SEXES::kMale];
        VampLordRace->faceRelatedData[RE::SEXES::kFemale] = VampireLordVanilla.faceRelatedData[RE::SEXES::kFemale];
        VampLordRace->morphRace = VampireLordVanilla.morphRace;
        VampLordRace->armorParentRace = VampireLordVanilla.armorParentRace;

        logger::info("Revert compatible patch for Vampire Lord of {} done", modname);
    }

    void RaceCompatibility::SolveCompatibleVampire()
    {
        SolveCompatibleVampireVanilla();
        SolveCompatibleVampireSacrosanct();
    }

    void RaceCompatibility::ChangeRaceHeight()
    {
        Config::GetSingleton().LoadExtraConfig();
        if (!Config::GetSingleton().GetVanillaScale())
            return;

        RE::TESRace* elin = GetElinRace(false);
        RE::TESRace* elinvamp = GetElinRace(true);

        if (!elin || !elinvamp)
            return;

        auto& elindata = elin->data;
        elindata.height[RE::SEXES::kMale] = 1.0f;
        elindata.height[RE::SEXES::kFemale] = 1.0f;

        auto& elinvampdata = elinvamp->data;
        elinvampdata.height[RE::SEXES::kMale] = 1.0f;
        elinvampdata.height[RE::SEXES::kFemale] = 1.0f;

        RE::TESForm* RunTimeHorizonRun = GetFormByID(HorizonRunID, TeraElinRaceESP);
        if (!RunTimeHorizonRun)
        {
            logger::error("Can't Find HorizonRun spell");
            return;
        }
        
        RE::SpellItem* HorizonRun = static_cast<RE::SpellItem*>(RunTimeHorizonRun);
        RE::MagicItem* magicitem = HorizonRun;
        auto& effects = magicitem->effects;
        if (2 != effects.size())
            return;

        effects[0]->effectItem.magnitude = 20;
        effects[1]->effectItem.magnitude = 15;

        logger::info("Change to vanilla scale done");
    }
}
