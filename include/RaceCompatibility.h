#pragma once

#include "ConsoleLog.h"

namespace Mus {
    static RE::BGSListForm* CompatibleHeadPartRaces = nullptr;

    class RaceCompatibility {
    public:
        [[nodiscard]] static RaceCompatibility& GetSingleton() {
            static RaceCompatibility instance;
            return instance;
        };

        void LoadRaceCompatibility();

        bool IsTeraElinRaceInstalled() {
            if (IsProblem)
                return false;

            RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
            auto* file = DataHandler->LookupLoadedModByName(TeraElinRaceESP);
            if (file)
            {
                logger::info("Detected the TeraElinRace installed");
                return true;
            }
            else
            {
                logger::error("Can't detect the TeraElinRace installed");
                return false;
            }
        };

        bool isPlayerRaceTeraElin();
        bool RemoveHeadPartElinRacesForm();
        bool InitFormList();
        bool GetRuntimeData();
        void SolveCompatibleVampire();
        void SolveCompatibleVampireLord();

        inline RE::TESRace* GetElinRace(bool isVamp) {
            RE::TESRace* result = nullptr;
            if (!isVamp)
            {
                RE::TESForm* ElinRaceForm = RE::TESForm::LookupByID(RunTimeTeraElinRaceFormID);
                if (ElinRaceForm)
                    result = reinterpret_cast<RE::TESRace*>(ElinRaceForm);
            }
            else
            {
                RE::TESForm* ElinRaceVampForm = RE::TESForm::LookupByID(RunTimeTeraElinRaceVampFormID);
                if (ElinRaceVampForm)
                    result = reinterpret_cast<RE::TESRace*>(ElinRaceVampForm);
            }
            return result;
        }
    private:
        bool isInitFormList = false;
        bool IsProblem = false;

        RE::BGSListForm* RunTimeHeadPartFormList;

        RE::BGSListForm* RunTimePlayableFormList;
        RE::BGSListForm* RunTimePlayableVampFormList;

        RE::FormID RunTimeTeraElinRaceFormID = NULL;
        RE::FormID RunTimeTeraElinRaceVampFormID = NULL;
        RE::FormID RunTimeTeraElinRaceFormListID = NULL;

        const RE::FormID HeadPartFormList = 0x000A803F;
        const RE::FormID ArgonianFormList = 0x000A8039;
        const RE::FormID KhajiitFormList = 0x000A8036;

        const RE::FormID VampireLordID = 0x0200283A;

        const RE::FormID PlayableFormList = 0x00000D62;
        const  RE::FormID PlayableVampFormList = 0x00000D63;
        const std::string_view RaceCompatibilityESP = "RaceCompatibility.esm";

        const RE::FormID TeraElinRaceFormID = 0x001000;
        const RE::FormID TeraElinRaceVampFormID = 0x001001;
        const RE::FormID TeraElinRaceFormListID = 0x001802;
        
        const std::string_view TeraElinRaceESP = "TeraElinRace.esm";

        const RE::FormID DefaultRaceID = 0x00000019;

        const RE::FormID CompatibleHeadPartRacesID = 0x046E35;

        std::vector<RE::FormID> VanillaHeadPartFormList;
        std::vector<RE::FormID> VanillaHeadPartFormExceptionList;

        std::vector<RE::FormID> doneHeadpartFormList;

        bool RaceController();
       
        bool CreateHeadPartFormList();
        void AddHeadPartRacesForm(RE::BGSListForm* formlist, bool isAddElin);
        void AddHeadPartBeastsRacesForm(RE::BGSListForm* formlist, bool isArgonian);
        void AddHeadPartVampireLordForm(RE::BGSListForm* formlist);
        bool SetNonePlayable(RE::BGSHeadPart* headpart);
        void GetVanillaFormList();
        bool ResolveCompatibleHairParts();

        inline bool IsBeastRace(RE::FormID id)
        {
            if (id == HeadPartExplorer::GetSingleton().Khajiit1 || id == HeadPartExplorer::GetSingleton().Khajiit2)
                return true;

            if (id == HeadPartExplorer::GetSingleton().Argonian1 || id == HeadPartExplorer::GetSingleton().Argonian2)
                return true;

            return false;
        }

        void SolveCompatibleVampireVanilla();
        const RE::FormID VampireQuest = 0x000EAFD5;
        void SolveCompatibleVampireSacrosanct();
        const std::string_view SacrosanctESP = "Sacrosanct - Vampires of Skyrim.esp";
        const RE::FormID SCS_RacesFormID = 0x00D5929;
        const RE::FormID SCS_RacesVampireFormID = 0x00D592A;

        const std::string_view HumanoidVampireLordESP = "HNVMain.esp";
        const std::string_view NoMoreUglyVampireLordESP = "NoMoreUglyVampireLord.esp";
        const std::string_view NoMoreUglyVampireLord2RVESP = "NoMoreUglyVampireLord_2RV.esp";
        const RE::FormID VampireLordRaceID = 0x0200283A;
    };

}  // namespace Mus
