#pragma once

namespace Mus {
    inline std::string GetRuntimeDirectory() 
    {
        char RuntimeDirectory[4096];
        if (_getcwd(RuntimeDirectory, 4096) == NULL)
        {
            SKSE::stl::report_and_fail("Unable to lookup Runtime directory.");
        }
        std::string Directory = RuntimeDirectory;
        Directory += "\\Data\\SKSE\\Plugins\\";

        return Directory;
    }

    inline RE::TESForm* GetFormByID(RE::FormID id, std::string_view modname) 
    {
        RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
        if (!DataHandler)
            return nullptr;

        auto esp = DataHandler->LookupModByName(modname);
        if (!esp)
            return nullptr;

        RE::FormID index = 0;

        if (REL::Module::IsVR() || !esp->IsLight())
        {
            RE::FormID Compileindex = esp->GetCompileIndex();
            index = Compileindex * 0x01000000;
        }
        else
        {
            RE::FormID Compileindex = esp->GetSmallFileCompileIndex();
            index = (Compileindex * 0x00001000) + 0xFE000000;
        }

        id += index;

        RE::TESForm* result = RE::TESForm::LookupByID(id);

        return result ? result : nullptr;
    }
    
    inline bool IsThereTheESP(std::string_view modname) 
    {
        RE::TESDataHandler* DataHandler = RE::TESDataHandler::GetSingleton();
        if (!DataHandler)
            return false;

        auto esp = DataHandler->LookupModByName(modname);

        return esp ? true : false;
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
        return id / 0x00001000;
    }

    inline bool IsLightMod(RE::FormID id)
    {
        return REL::Module::IsVR() ? false : (id >= 0xFE000000); //VR hasn't esl
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

    inline std::string spaces(int n) {
        auto s = std::string(n, ' ');
        return s;
    }

    inline bool isPlayer(RE::FormID id) { return (id == 0x14 || id == 0x7); };

    inline std::string lowLetter(std::string_view Letter)
    {
        std::string Result = Letter.data();
        for (size_t i = 0; i < Result.size(); i++) {
            Result[i] = tolower(Result[i]);
        }
        return Result;
    }

    inline bool IsMatchName(std::string base, std::string find)
    {
        std::string baselow = lowLetter(base.c_str());
        std::string findlow = lowLetter(find.c_str());

        return (baselow.find(findlow) != std::string::npos);
    }
}
