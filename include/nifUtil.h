#pragma once

namespace Mus::nif {
    bool visitObjects(RE::NiAVObject* parent, std::function<bool(RE::NiAVObject*, int)> functor, int depth = 0) {
        if (!parent) return false;
        RE::NiNode* node = reinterpret_cast<RE::NiNode*>(parent);
        if (node) {
            if (functor(parent, depth))
                return true;

            for (std::uint16_t i = 0; i < node->GetChildren().size(); i++) {
                RE::NiAVObject* object = node->GetChildren()[i] ? node->GetChildren()[i].get() : nullptr;
                if (object) {
                    if (visitObjects(object, functor, depth + 1))
                        return true;
                }
            }
        }
        else if (functor(parent, depth))
            return true;

        return false;
    };

    bool printStuff(RE::NiAVObject* avObj, int depth);

    class TaskupdateNode : public SKSE::TaskInterface
    {
    public:
        TaskupdateNode(RE::NiAVObject* node, bool isDawnpass) : obj(node), dawnpass(isDawnpass) { }

        virtual void Run();
        virtual void Dispose() { delete this; }

        RE::NiAVObject* obj;
        bool dawnpass;
    };
    
    class nodeTaskInterface : public SKSE::TaskInterface
    {
    public:
        [[nodiscard]] static nodeTaskInterface& GetSingleton() {
            static nodeTaskInterface instance;
            return instance;
        }

        void AddnewNodeTask(RE::NiAVObject* node, bool instant, bool isDawnpass);
    };

    inline void setBSFixedString(RE::BSFixedString& ref, const char* name)
    {
        ref = name;
    }

    inline void setNiNodeName(RE::NiNode* node, const char* name)
    {
        setBSFixedString(node->name, name);
    }

    RE::NiAVObject* addParentToNode(RE::NiAVObject* node, const char* name);

    RE::BSFixedString GetVirtualNodeName(RE::BSFixedString nodeName);
}