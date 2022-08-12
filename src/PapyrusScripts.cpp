#include "PapyrusScripts.h"

namespace Mus {
    namespace Papyrus {
        RE::BSFixedString GetHeadNodeName(RE::StaticFunctionTag*, RE::Actor* actor, RE::BSFixedString FindName) {
            RE::BSFixedString result("");

            if (!actor)
                return result;

			RE::BSFaceGenNiNode* FaceNode = actor->GetFaceNodeSkinned();

			if (!FaceNode)
				return result;

			RE::NiNode* node = FaceNode->AsNode();
			if (!node)
				return result;

			std::string findstr = lowLetter(FindName.c_str());
			for (auto& child : node->GetChildren())
			{
				auto obj = child.get();
				if (!obj || !obj->name.c_str())
					continue;

				std::string nodeName = lowLetter(obj->name.c_str());
				if (nodeName.find(findstr) != std::string::npos)
				{
					nif::setBSFixedString(result, obj->name.c_str());
					return result;
				}
			}

            return result;
        }

        bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
            vm->RegisterFunction("GetHeadNodeName", ScriptFileName, GetHeadNodeName);
            return true;
        }
    }
}  // namespace Mus