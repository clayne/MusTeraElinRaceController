#include "nifUtil.h"

namespace Mus::nif {
    bool visitObjects(RE::NiAVObject* a_object, std::function<bool(RE::NiAVObject*, int)> a_func, int depth){
        if (!a_object) return true;

        auto result = a_func(a_object, depth);
        if (!result)
            return result;

        result = true;
        auto node = a_object->AsNode();
        if (node) {
            for (auto& child : node->GetChildren()) {
                result = visitObjects(child.get(), a_func, depth + 1);
                if (!result) {
                    break;
                }
            }
        }

        return result;
    }

    bool printStuff(RE::NiAVObject* a_object, int depth) {
        if (!a_object) return true;
        std::string sss = spaces(depth);
        const char* ss = sss.c_str();
        logger::debug("{}avObj Name = {}, RTTI = {}", ss, a_object->name.c_str(), a_object->GetRTTI() ? a_object->GetRTTI()->name : "");

        auto node = a_object->AsNode();
        if (node) {
            logger::debug("{}node {}, RTTI {}", ss, node->name.c_str(), node->GetRTTI() ? a_object->GetRTTI()->name : "");
        }
        return true;
    }

    RE::BSVisit::BSVisitControl printStuffAlt(RE::NiAVObject* a_object) {
        if (!a_object) return RE::BSVisit::BSVisitControl::kContinue;
        logger::debug("avObj Name = {}, RTTI = {}", a_object->name.c_str(), a_object->GetRTTI() ? a_object->GetRTTI()->name : "");

        auto node = a_object->AsNode();
        if (node) {
            logger::debug("node {}, RTTI {}", node->name.c_str(), node->GetRTTI() ? a_object->GetRTTI()->name : "");
        }
        return RE::BSVisit::BSVisitControl::kContinue;
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