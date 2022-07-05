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
        auto* P = RE::PlayerCharacter::GetSingleton();
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

        CompatibleHeadPartRaces = reinterpret_cast<RE::BGSListForm*>(RunTimeHeadPartFormList->CreateDuplicateForm(true, (void*)CompatibleHeadPartRaces));
        if (!CompatibleHeadPartRaces)
            return false;

        logger::trace("Create NewHeadPartFormList finished");
        return true;
    };

    void RaceCompatibility::AddHeadPartRacesForm() 
    {
        auto& HeadPartFormList = RunTimeHeadPartFormList->forms;
        for (int index = 0; index < HeadPartFormList.size(); index++) 
        {
            if (HeadPartFormList[index])
                CompatibleHeadPartRaces->AddForm(HeadPartFormList[index]);
        }

        CompatibleHeadPartRaces->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID));
        CompatibleHeadPartRaces->AddForm(RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID));
    };

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

        AddHeadPartRacesForm();

        concurrency::parallel_for_each(HPE.HeadPartFormMap.begin(), HPE.HeadPartFormMap.end(), [&](auto& HP) { 
            if (HP.second->validRaces)
                HP.second->validRaces = CompatibleHeadPartRaces;

            if (HP.second->validRaces->formID != CompatibleHeadPartRaces->formID)
                logger::error("Can't edit headpart for race compatible : {:x}", HP.second->validRaces->formID);
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
