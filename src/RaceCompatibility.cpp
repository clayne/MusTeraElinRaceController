#include "RaceCompatibility.h"

namespace Mus {
    bool RaceCompatibility::RaceController() 
    { 
        if (!GetRuntimeData()) return false;

        //if (!isRaceTeraElin()) return false;

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
            return false;
        }

        RE::TESForm* RunTimeTeraElinRaceForm = GetFormByID(TeraElinRaceFormID, TeraElinRaceESP);
        RE::TESForm* RunTimeTeraElinRaceVampForm = GetFormByID(TeraElinRaceVampFormID, TeraElinRaceESP);
        if (!RunTimeTeraElinRaceForm || !RunTimeTeraElinRaceVampForm)
        {
            logger::error("Can't Find Tera Elin Race ID");
            return false;
        }
        RunTimeTeraElinRaceFormID = RunTimeTeraElinRaceForm->formID;
        RunTimeTeraElinRaceVampFormID = RunTimeTeraElinRaceVampForm->formID;

        logger::trace("Building RuntimeData finished");

        return true;
    };

    bool RaceCompatibility::isPlayerRaceTeraElin()
    {
        RE::PlayerCharacter* P = RE::PlayerCharacter::GetSingleton();

        if (!P)
            return false;

        if (P->formID == RunTimeTeraElinRaceFormID || P->formID == RunTimeTeraElinRaceVampFormID) 
        {
            logger::info("Detected that Player Race is TeraElinRace");
            return true;
        }
        return false;
    };

    bool RaceCompatibility::CreateHeadPartFormList()
    {
        logger::trace("Creating NewHeadPartFormList...");

        if (!RunTimeHeadPartFormList) 
            return false;

        if (!RunTimeHeadPartFormList->formType.all(RE::FormType::FormList)) 
            return false;

        if (!(CompatibleHeadPartRaces = reinterpret_cast<RE::BGSListForm*>(RunTimeHeadPartFormList->CreateDuplicateForm(true, (void*)CompatibleHeadPartRaces))))
            return false;

        logger::trace("Create NewHeadPartFormList finished");
        return true;
    };

    bool RaceCompatibility::SetNonePlayable(RE::BGSHeadPart* headpart)
    {
        if (!headpart || !HeadPartExplorer::GetSingleton().IsPlayerble(headpart->flags))
            return false;

        auto& flag = headpart->flags;
        flag -= RE::BGSHeadPart::Flag::kPlayable;
        return true;
    }

    void RaceCompatibility::AddHeadPartRacesForm(RE::BGSListForm* formlist, bool isAddElin)
    {
        auto& HeadPartFormList = RunTimeHeadPartFormList->forms;
        for (RE::BSTArrayBase::size_type index = 0; index < HeadPartFormList.size(); index++) 
        {
            if (HeadPartFormList[index])
                formlist->AddForm(HeadPartFormList[index]);
        }

        auto* argonian = reinterpret_cast<RE::BGSListForm*>(RE::TESForm::LookupByID(ArgonianFormList));
        if (argonian)
        {
            for (RE::BSTArrayBase::size_type index = 0; index < argonian->forms.size(); index++)
            {
                if (argonian->forms[index])
                    formlist->AddForm(argonian->forms[index]);
            }
        }

        auto* khajiit = reinterpret_cast<RE::BGSListForm*>(RE::TESForm::LookupByID(KhajiitFormList));
        if (khajiit)
        {
            for (RE::BSTArrayBase::size_type index = 0; index < khajiit->forms.size(); index++)
            {
                if (khajiit->forms[index])
                    formlist->AddForm(khajiit->forms[index]);
            }
        }

        if (isAddElin)
        {
            CompatibleHeadPartRaces->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID));
            CompatibleHeadPartRaces->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID));
        }
    };

    bool RaceCompatibility::RemoveHeadPartElinRacesForm()
    {
        if (!Config::GetSingleton().GetSetting().GetFeature().GetBeforeSaveCompatible() || !Config::GetSingleton().GetSetting().GetFeature().GetRaceController() || IsPlayerElin)
            return false;

        auto& HPE = HeadPartExplorer::GetSingleton();

        logger::info("Try to cleanup save script trashes... {}", HPE.OtherHeadPartFormMap.size());

        std::vector<RE::FormID> doneHeadpartFormList;
        for (auto& h : HPE.OtherHeadPartFormMap)
        {
            if (!h.second || !h.second->validRaces)
                continue;

            if (std::find(doneHeadpartFormList.begin(), doneHeadpartFormList.end(), h.second->validRaces->formID) != doneHeadpartFormList.end())
                continue;

            auto* list = h.second->validRaces;
            bool isRemoved = false;
            if (list && (list->HasForm(RunTimeTeraElinRaceFormID) || list->HasForm(RunTimeTeraElinRaceVampFormID)))
            {
                list->ClearData();
                AddHeadPartRacesForm(list, false);
                isRemoved = true;
            }

            if (isRemoved)
            {
                doneHeadpartFormList.push_back(list->formID);
                logger::trace("Removed elin races from {:x} formlist", list->formID);
            }
        }

        return true;
    }

    bool RaceCompatibility::ResolveCompatibleHairParts() 
    {
        logger::trace("Try resolve Compatible issues in HeadParts...");
        auto& HPE = HeadPartExplorer::GetSingleton();
        if (!HPE.InitFormList()) 
        {
            logger::error("Can't Initial headpart form map");
            return false;
        }

        if (!CreateHeadPartFormList())
        {
            logger::error("Can't Create New HeadPart ValidRaces List");
            return false;
        }

        AddHeadPartRacesForm(CompatibleHeadPartRaces, true);

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
        if (!Config::GetSingleton().GetSetting().GetFeature().GetRaceController() || !IsValidTeraElinRace)
        {
            logger::info("Disabled Race Compatibility Controller");
        }

        logger::trace("RaceController Loading...");
        if (RaceController()) 
            logger::info("RaceController finished");
        else
            logger::error("Can't load the RaceController");
        return;
    }
}
