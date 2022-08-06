#pragma once

namespace Mus::nif {
    bool visitObjects(RE::NiAVObject* a_object, std::function<bool(RE::NiAVObject*, int)> a_func, int depth = 0);

    bool printStuff(RE::NiAVObject* a_object, int depth);
    RE::BSVisit::BSVisitControl printStuffAlt(RE::NiAVObject* a_object);

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