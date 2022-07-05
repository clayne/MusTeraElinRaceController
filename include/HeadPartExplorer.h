#pragma once

namespace Mus {
    class HeadPartExplorer {
    public:
        [[nodiscard]] static HeadPartExplorer& GetSingleton() {
            static HeadPartExplorer instance;
            return instance;
        };
        
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> HeadPartFormMap;
        concurrency::concurrent_unordered_map<RE::FormID, RE::BGSHeadPart*> RemoveHeadPartFormMap;

        bool InitFormList();

    private:
        using HeadPartFlag = RE::BGSHeadPart::Flag;
        using HeadPartType = RE::BGSHeadPart::HeadPartType;
        using enumHeadPartFlag = SKSE::stl::enumeration<RE::BGSHeadPart::Flag, std::uint8_t>;
        using enumHeadPartType = SKSE::stl::enumeration<RE::BGSHeadPart::HeadPartType, std::uint32_t>;

        inline bool IsPlayerble(enumHeadPartFlag flags) { return flags.all(HeadPartFlag::kPlayable); };
        inline bool IsHairPart(enumHeadPartType type) { return (type == HeadPartType::kHair); };
        inline bool IsNotForCustomRaceOnly(RE::BSTArray<RE::TESForm*> &validRaces) 
        { 
            bool isVanilla = false;
            for (int index = 0; index < validRaces.size(); index++) 
            {
                if (validRaces[index] && validRaces[index]->formID < 0x05000000)
                    isVanilla = true;
            }
            return isVanilla;
        };
    };

}
