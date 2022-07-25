#pragma once

namespace Mus {
    class HeadPartExplorer {
        using HeadPartFlag = RE::BGSHeadPart::Flag;
        using HeadPartType = RE::BGSHeadPart::HeadPartType;
        using enumHeadPartFlag = SKSE::stl::enumeration<RE::BGSHeadPart::Flag, std::uint8_t>;
        using enumHeadPartType = SKSE::stl::enumeration<RE::BGSHeadPart::HeadPartType, std::uint32_t>;

    public:
        [[nodiscard]] static HeadPartExplorer& GetSingleton() {
            static HeadPartExplorer instance;
            return instance;
        };
        
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> HeadPartFormMap;
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> OtherHeadPartFormMap;
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> NoneRacesHeadPartFormMap;

        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> KhajiitRacesHeadPartFormMap;
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> ArgonianRacesHeadPartFormMap;

        bool InitFormList();

        const RE::FormID Khajiit1 = 0x13745;
        const RE::FormID Khajiit2 = 0x88845;
        const RE::FormID Argonian1 = 0x13740;
        const RE::FormID Argonian2 = 0x8883a;

        inline bool IsPlayerble(enumHeadPartFlag flags) { return flags.all(HeadPartFlag::kPlayable); };
        inline bool IsExtra(enumHeadPartFlag flags) { return flags.all(HeadPartFlag::kIsExtraPart); };

        inline bool IsHairPart(enumHeadPartType type) { return (type == HeadPartType::kHair); };
        inline bool IsEyebrowsPart(enumHeadPartType type) { return (type == HeadPartType::kEyebrows); };
        inline bool IsScarPart(enumHeadPartType type) { return (type == HeadPartType::kScar); };
        inline bool IsFacialHairPart(enumHeadPartType type) { return (type == HeadPartType::kFacialHair); };
        inline bool IsEyesPart(enumHeadPartType type) { return (type == HeadPartType::kEyes); };
        inline bool IsFacePart(enumHeadPartType type) { return (type == HeadPartType::kFace); };
        inline bool IsMiscPart(enumHeadPartType type) { return (type == HeadPartType::kMisc); };

        inline bool IsNotForCustomRaceOnly(RE::BSTArray<RE::TESForm*>& validRaces)
        {
            bool isVanilla = false;
            for (RE::BSTArrayBase::size_type index = 0; index < validRaces.size(); index++)
            {
                if (validRaces[index] && validRaces[index]->formID < 0x05000000)
                    isVanilla = true;
            }
            return isVanilla;
        };

        inline bool IsForKhajiitRace(RE::BSTArray<RE::TESForm*>& validRaces)
        {
            bool isForRace = true;
            for (RE::BSTArrayBase::size_type index = 0; index < validRaces.size(); index++)
            {
                if (validRaces[index] && (validRaces[index]->formID != Khajiit1 && validRaces[index]->formID != Khajiit2))
                    isForRace = false;
            }
            return isForRace;
        };

        inline bool IsForArgonianRace(RE::BSTArray<RE::TESForm*>& validRaces)
        {
            bool isForRace = true;
            for (RE::BSTArrayBase::size_type index = 0; index < validRaces.size(); index++)
            {
                if (validRaces[index] && (validRaces[index]->formID != Argonian1 && validRaces[index]->formID != Argonian2))
                    isForRace = false;
            }
            return isForRace;
        };

    private:
        void GetHeadPartConfig();

        bool EyeBrows = false;
        bool Scar = false;
        bool FacialHair = false;
        bool Eyes = false;
        bool Face = false;
        bool Misc = false;

        inline bool IsExceptionHeadPart(enumHeadPartType type)
        {
            if (IsEyebrowsPart(type) && EyeBrows)
                return true;
            else if (IsScarPart(type) && Scar)
                return true;
            else if (IsFacialHairPart(type) && FacialHair)
                return true;
            else if (IsEyesPart(type) && Eyes)
                return true;
            else if (IsFacePart(type) && Face)
                return true;
            else if (IsMiscPart(type) && Misc)
                return true;
            
            return false;
        };
    };

}
