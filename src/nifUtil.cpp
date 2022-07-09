#include "nifUtil.h"

namespace Mus::nif {
    bool printStuff(RE::NiAVObject* avObj, int depth) {
        if (!avObj) return false;
        std::string sss = spaces(depth);
        const char* ss = sss.c_str();
        logger::debug("{}avObj Name = {}, RTTI = {}", ss, avObj->name.c_str(), avObj->GetRTTI() ? avObj->GetRTTI()->name : "");

        RE::NiNode* node = reinterpret_cast<RE::NiNode*>(avObj);
        if (node) {
            logger::debug("{}node {}, RTTI {}", ss, node->name.c_str(), node->GetRTTI() ? avObj->GetRTTI()->name : "");
        }
        return false;
    }

    void TaskupdateNode::Run() {
        RE::NiUpdateData ctx;
        ctx.flags = RE::NiUpdateData::Flag::kDirty;
        ctx.time = 0;
        if (dawnpass)
            obj->IUpdateDownwardPass(ctx, 0x0);
        else
            obj->IUpdateWorldData(&ctx);
    }

    void nodeTaskInterface::AddnewNodeTask(RE::NiAVObject* node, bool instant, bool isDawnpass) {
        const SKSE::detail::SKSETaskInterface* g_task = SKSE::TaskInterface::GetProxy();
        nif::TaskupdateNode* newTask = new nif::TaskupdateNode(node, isDawnpass);

        if (g_task && !instant)
        {
            g_task->AddTask(newTask);
        }
        else
        {
            newTask->Run();
            newTask->Dispose();
        }
    }

    RE::NiAVObject* addParentToNode(RE::NiAVObject* node, const char* name)
    {
        if (!node)
            return nullptr;

        auto parent = node->parent;
        auto newParent = RE::NiNode::Create(1);
        node->IncRefCount();
        if (parent)
        {
            parent->DetachChild(node);
            parent->AttachChild(newParent, true);
        }
        newParent->AttachChild(node, true);
        setNiNodeName(newParent, name);
        node->DecRefCount();
        return newParent;
    }

    RE::BSFixedString GetVirtualNodeName(RE::BSFixedString nodeName)
    {
        std::string name = NodePrefix;
        name += nodeName.c_str();
        RE::BSFixedString newName;
        setBSFixedString(newName, name.c_str());
        return newName;
    }
}