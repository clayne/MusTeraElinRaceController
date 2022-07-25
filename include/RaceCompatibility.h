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
    private:
        bool isInitFormList = false;
        bool IsProblem = false;

        RE::BGSListForm* RunTimeHeadPartFormList;

        RE::BGSListForm* RunTimePlayableFormList;
        RE::BGSListForm* RunTimePlayableVampFormList;

        RE::FormID RunTimeTeraElinRaceFormID = NULL;
        RE::FormID RunTimeTeraElinRaceVampFormID = NULL;

        RE::FormID HeadPartFormList = 0x000A803F;
        RE::FormID ArgonianFormList = 0x000A8039;
        RE::FormID KhajiitFormList = 0x000A8036;

        RE::FormID PlayableFormList = 0x00000D62;
        RE::FormID PlayableVampFormList = 0x00000D63;
        std::string_view RaceCompatibilityESP = "RaceCompatibility.esm";

        RE::FormID TeraElinRaceFormID = 0x001000;
        RE::FormID TeraElinRaceVampFormID = 0x001001;

        RE::FormID CompatibleHeadPartRacesID = 0x046E35;
        std::string_view TeraElinRaceESP = "TeraElinRace.esm";

        std::vector<RE::FormID> VanillaHeadPartFormList;
        std::vector<RE::FormID> VanillaHeadPartFormExceptionList;

        std::vector<RE::FormID> doneHeadpartFormList;

        bool RaceController();
       
        bool CreateHeadPartFormList();
        void AddHeadPartRacesForm(RE::BGSListForm* formlist, bool isAddElin);
        void AddHeadPartBeastsRacesForm(RE::BGSListForm* formlist, bool isArgonian);
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

        //bool IsHeadPartForBeasts();
    };

}  // namespace Mus
