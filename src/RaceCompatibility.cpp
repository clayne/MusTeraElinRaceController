#include "RaceCompatibility.h"

namespace Mus {
    bool RaceCompatibility::RaceController() 
    { 
        IsPlayerElin.store(isPlayerRaceTeraElin());

        if (!IsPlayerElin.load()) return false;

        logger::trace("RaceCompatibility Loading...");
        return ResolveCompatibleHairParts();
    }

    bool RaceCompatibility::GetRuntimeData() 
    {
        logger::trace("Building RuntimeData...");

        RunTimeHeadPartFormList = reinterpret_cast<RE::BGSListForm*>(RE::TESForm::LookupByID(HeadPartFormList));
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

        RE::TESForm* RunTimePlayableForm = GetFormByID(PlayableFormList, RaceCompatibilityESP);
        if (!RunTimePlayableForm)
        {
            logger::error("Can't Find RaceCompatibilityFormList");
            IsProblem = true;
            return false;
        }
        RunTimePlayableFormList = reinterpret_cast<RE::BGSListForm*>(RunTimePlayableForm);

        RE::TESForm* RunTimePlayableVampForm = GetFormByID(PlayableVampFormList, RaceCompatibilityESP);
        if (!RunTimePlayableVampForm)
        {
            logger::error("Can't Find RaceCompatibilityFormList");
            IsProblem = true;
            return false;
        }
        RunTimePlayableVampFormList = reinterpret_cast<RE::BGSListForm*>(RunTimePlayableVampForm);

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

        if (P->GetActorBase()->GetRace()->formID == RunTimeTeraElinRaceFormID || P->GetActorBase()->GetRace()->formID == RunTimeTeraElinRaceVampFormID)
        {
            logger::info("Detected that Player Race is TeraElinRace");
            return true;
        }
        return false;
    }

    bool RaceCompatibility::CreateHeadPartFormList()
    {
        logger::trace("Creating NewHeadPartFormList...");

        if (!RunTimeHeadPartFormList) 
            return false;

        if (!RunTimeHeadPartFormList->formType.all(RE::FormType::FormList)) 
            return false;

        if (!(CompatibleHeadPartRaces = reinterpret_cast<RE::BGSListForm*>(RunTimeHeadPartFormList->CreateDuplicateForm(true, (void*)CompatibleHeadPartRaces))))
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

    bool RaceCompatibility::RemoveHeadPartElinRacesForm()
    {
        if (IsProblem)
            return false;

        if (!Config::GetSingleton().GetSetting().GetFeature().GetBeforeSaveCompatible() || !Config::GetSingleton().GetSetting().GetFeature().GetRaceController() || !IsPlayerElin.load())
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

        logger::trace("Try resolve Compatible issues in HeadParts finished");
        return true;
    }

    void RaceCompatibility::LoadRaceCompatibility()
    {
        if (IsProblem)
            return;

        if (!Config::GetSingleton().GetSetting().GetFeature().GetRaceController() || !IsValidTeraElinRace)
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
}
