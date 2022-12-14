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

            if (IsThereTheESP(TeraElinRaceESP))
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
        bool isPlayerRaceTeraElinHumanoidVampireLord();
        bool RemoveHeadPartElinRacesForm();
        bool InitFormList();
        bool GetRuntimeData();
        void SolveCompatibleVampire();
        void SolveCompatibleVampireLord();
        void RevertCompatibleVampireLord();
        void ChangeRaceHeight();

        inline RE::TESRace* GetRace(RE::FormID id) {
            RE::TESRace* result = nullptr;
            RE::TESForm* RaceForm = RE::TESForm::LookupByID(id);
            if (RaceForm)
            {
                if (RaceForm->formType.get() != RE::FormType::Race)
                    return nullptr;
                else
                    result = static_cast<RE::TESRace*>(RaceForm);
            }
            return result;
        }

        inline RE::TESRace* GetElinRace(bool isVamp) {
            if (IsProblem)
                return nullptr;

            RE::TESRace* result = nullptr;
            if (!isVamp)
            {
               result = static_cast<RE::TESRace*>(GetRace(RunTimeTeraElinRaceFormID));
            }
            else
            {
                 result = static_cast<RE::TESRace*>(GetRace(RunTimeTeraElinRaceVampFormID));
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

        struct VampireLordRecord {
            RE::TESObjectARMO* skin = nullptr;
            float height[RE::SEXES::kTotal] = {0.0f, 0.0f};
            bool kFaceGenHead = false;
            bool kOverlayHeadPartList = false;
            bool kOverrideHeadPartList = false;
            RE::BSFixedString skeletonModels[RE::SEXES::kTotal];
            RE::TESRace::FaceRelatedData* faceRelatedData[RE::SEXES::kTotal];
            RE::TESRace* morphRace;
            RE::TESRace* armorParentRace;
        };

        VampireLordRecord VampireLordVanilla;

        const RE::FormID HorizonRunID = 0x000145DF;
    };

}  // namespace Mus
