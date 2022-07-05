#pragma once

namespace Mus {
    constexpr std::string_view ScriptFileName = "TeraElinRaceController";

    bool RegisterElinAnimation(RE::StaticFunctionTag*, RE::Actor* actor);

    bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* vm);
}