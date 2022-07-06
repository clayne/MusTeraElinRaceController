#pragma once

#include "ConsoleLog.h"

namespace Mus {
    static RE::BGSListForm* CompatibleHeadPartRaces;

    class RaceCompatibility {
    public:
        [[nodiscard]] static RaceCompatibility& GetSingleton() {
            static RaceCompatibility instance;
            return instance;
        };

        void LoadRaceCompatibility();

        bool IsTeraElinRaceInstalled() {
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
    private:
        RE::BGSListForm* RunTimeHeadPartFormList;

        RE::FormID RunTimeTeraElinRaceFormID = NULL;
        RE::FormID RunTimeTeraElinRaceVampFormID = NULL;

        RE::FormID HeadPartFormList = 0x000A803F;
        RE::FormID ArgonianFormList = 0x000A8039;
        RE::FormID KhajiitFormList = 0x000A8036;

        RE::FormID TeraElinRaceFormID = 0x001000;
        RE::FormID TeraElinRaceVampFormID = 0x001001;

        RE::FormID CompatibleHeadPartRacesID = 0x046E35;
        std::string_view TeraElinRaceESP = "TeraElinRace.esm";

        bool RaceController();
        bool GetRuntimeData();
        bool CreateHeadPartFormList();
        void AddHeadPartRacesForm(RE::BGSListForm* formlist, bool isAddElin);
        bool SetNonePlayable(RE::BGSHeadPart* headpart);
        bool ResolveCompatibleHairParts();
    };

}  // namespace Mus
