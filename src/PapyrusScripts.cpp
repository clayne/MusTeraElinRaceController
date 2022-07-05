#include "PapyrusScripts.h"

namespace Mus {
    bool RegisterElinAnimation(RE::StaticFunctionTag*, RE::Actor* actor) {
        auto* am = ActorManager::instance();
        if (!am)
            return false;

        //am->RegisterActor(actor);
        return true;
    }

    bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
       // vm->RegisterFunction("RegisterElinAnimation", ScriptFileName, RegisterElinAnimation);
        return true;
    }
}  // namespace Mus