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

        bool InitFormList();

        inline bool IsPlayerble(enumHeadPartFlag flags) { return flags.all(HeadPartFlag::kPlayable); };
        inline bool IsHairPart(enumHeadPartType type) { return (type == HeadPartType::kHair); };
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
    private:
       
    };

}
