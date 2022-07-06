#pragma once

namespace Mus {
    // trim from start (in place)
    inline void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [](int c) {return !std::isspace(c); }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [](int c) {return !std::isspace(c); }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string& s)
    {
        ltrim(s);
        rtrim(s);
    }

    inline std::string trim_copy(std::string s)
    {
        trim(s);
        return s;
    }

    static inline std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::string str = trim_copy(s);

        std::vector<std::string> tokens;
        if (!str.empty())
        {
            std::string token;
            std::istringstream tokenStream(str);
            while (std::getline(tokenStream, token, delimiter))
            {
                trim(token);
                tokens.emplace_back(token);
            }
        }
        return tokens;
    }

    inline void skipComments(std::string& str)
    {
        auto pos = str.find("#");
        if (pos != std::string::npos)
        {
            str.erase(pos);
        }
    }

    inline std::uint32_t getHex(std::string hexstr)
    {
        return (std::uint32_t)strtoul(hexstr.c_str(), 0, 16);
    }

    inline RE::TESForm* GetFormByID(RE::FormID id, std::string_view modname) 
    {
        RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
        return DataHandler ? DataHandler->LookupForm(id, modname) : nullptr;
    }

    inline std::uint32_t GetActorBaseFormID(RE::Actor* actor)
    {
        if (!actor)
            return 0;

        RE::TESNPC* actorBase = actor->GetActorBase();
        
        return actorBase ? actorBase->formID : 0;
    }

    inline std::uint8_t GetModIndex(RE::FormID id)
    {
        return id / 0x01000000;
    }

    inline std::uint16_t GetLightModIndex(RE::FormID id)
    {
        return id / 0x00100000;
    }

    inline bool IsLightMod(RE::FormID id)
    {
        return id >= 0xFE000000;
    }

    inline std::string_view GetModNameByIndex(std::uint8_t index)
    {
        RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
        return (DataHandler && DataHandler->LookupLoadedModByIndex(index)) ? DataHandler->LookupLoadedModByIndex(index)->GetFilename() : "";
    }

    inline std::string_view GetLightModNameByIndex(std::uint16_t index)
    {
        RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
        return (DataHandler && DataHandler->LookupLoadedLightModByIndex(index)) ? DataHandler->LookupLoadedLightModByIndex(index)->GetFilename() : "";
    }

    inline std::string_view GetModNameByID(RE::FormID id) //if modindex is 0xFF then it'll return "Template"
    {
        if (id >= 0xFF000000)
            return "Template";

        if (!IsLightMod(id))
            return GetModNameByIndex(GetModIndex(id));
        else
            return GetLightModNameByIndex(GetLightModIndex(id));
    }

    inline std::vector<std::string> get_all_files_names_within_folder(std::string folder)
    {
        std::vector<std::string> names;

        DIR* directory = opendir(folder.c_str());
        struct dirent* direntStruct;

        if (directory != nullptr) {
            while (direntStruct = readdir(directory)) {
                names.emplace_back(direntStruct->d_name);
            }
        }
        closedir(directory);

        return names;
    }

    inline bool stringStartsWith(std::string str, std::string prefix)
    {
        // convert string to back to lower case
        std::for_each(str.begin(), str.end(), [](char& c)
            {
                c = ::tolower(c);
            });
        // std::string::find returns 0 if toMatch is found at starting
        if (str.find(prefix) == 0)
            return true;
        else
            return false;
    }

    inline bool stringEndsWith(std::string str, std::string const& suffix)
    {
        std::for_each(str.begin(), str.end(), [](char& c)
            {
                c = ::tolower(c);
            });
        if (str.length() >= suffix.length())
        {
            return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
        }
        else
        {
            return false;
        }
    }

    inline std::string spaces(int n) {
        auto s = std::string(n, ' ');
        return s;
    }

    namespace nif {
        inline bool visitObjects(RE::NiAVObject* parent, std::function<bool(RE::NiAVObject*, int)> functor, int depth = 0) {
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
        }

        inline bool printStuff(RE::NiAVObject* avObj, int depth) {
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

        class TaskupdateNode : public SKSE::TaskInterface
        {
        public:
            TaskupdateNode(RE::NiAVObject* node, bool isDawnpass) : obj(node), dawnpass(isDawnpass) { }

            virtual void Run() {
                RE::NiUpdateData ctx;
                ctx.flags = RE::NiUpdateData::Flag::kDirty;
                ctx.time = 0;
                if (dawnpass)
                    obj->UpdateDownwardPass(ctx, 0x0);
                else
                    obj->UpdateWorldData(&ctx);
            };
            virtual void Dispose() { delete this; }

            RE::NiAVObject* obj;
            bool dawnpass;
        };

        inline void setBSFixedString(RE::BSFixedString& ref, const char* name)
        {
            ref = name;
        }

        inline void setNiNodeName(RE::NiNode* node, const char* name)
        {
            setBSFixedString(node->name, name);
        }

        inline RE::NiAVObject* addParentToNode(RE::NiAVObject* node, const char* name)
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

        inline const RE::BSFixedString GetVirtualNodeName(RE::BSFixedString nodeName)
        {
            std::string name = UsePrefix;
            name += nodeName.c_str();
            RE::BSFixedString newName(name.c_str());
            return newName;
        }
    }
}
