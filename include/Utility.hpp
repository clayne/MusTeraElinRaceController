#pragma once

namespace Mus {
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
}
