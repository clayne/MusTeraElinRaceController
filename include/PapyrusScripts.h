#pragma once

namespace Mus {
    namespace Papyrus {
        constexpr std::string_view ScriptFileName = "TeraElinRaceController";
        RE::BSFixedString GetHeadNodeName(RE::StaticFunctionTag*, RE::Actor* actor, RE::BSFixedString FindName);
        bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* vm);
    }
}