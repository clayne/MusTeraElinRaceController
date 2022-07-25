#include "HeadPartExplorer.h"

namespace Mus {
    bool HeadPartExplorer::InitFormList() {
        logger::info("Init and building FormMap...");

        GetHeadPartConfig();

        auto handler = RE::TESDataHandler::GetSingleton();
        if (!handler) {
            logger::error("Missing TESDataHandler!");
            return false;
        }

        auto& items = handler->GetFormArray(RE::FormType::HeadPart);

        concurrency::parallel_for(RE::BSTArrayBase::size_type(0), items.size(), [&](RE::BSTArrayBase::size_type index) {
            if (items[index]) 
            {
                RE::FormID formID = items[index]->formID;
                RE::BGSHeadPart* data = reinterpret_cast<RE::BGSHeadPart*>(items[index]);

                if (data && IsPlayerble(data->flags)) {
                    if (!data->validRaces && !IsHairPart(data->type))
                    {
                        NoneRacesHeadPartFormMap.insert(std::make_pair(formID, data));
                        logger::trace("found and insert {} {} {:x} haven't validraces {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                    }
                    else if (data->validRaces && IsNotForCustomRaceOnly(data->validRaces->forms))
                    {
                        if (IsHairPart(data->type) || IsExtra(data->flags) || IsExceptionHeadPart(data->type))
                        {
                            HeadPartFormMap.insert(std::make_pair(formID, data));
                            logger::trace("found and insert {} {} {:x} hair {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                        }
                        else if (Config::GetSingleton().GetSetting().GetFeature().GetRaceController() && Config::GetSingleton().GetSetting().GetFeature().GetBeforeSaveCompatible())
                        {
                            if (IsForKhajiitRace(data->validRaces->forms))
                            {
                                KhajiitRacesHeadPartFormMap.insert(std::make_pair(formID, data));
                                logger::trace("found and insert {} {} {:x} hair {}on Khajiit formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                            }
                            else if (IsForArgonianRace(data->validRaces->forms))
                            {
                                ArgonianRacesHeadPartFormMap.insert(std::make_pair(formID, data));
                                logger::trace("found and insert {} {} {:x} hair {}on Argonian1 formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                            }
                            else
                            {
                                OtherHeadPartFormMap.insert(std::make_pair(formID, data));
                                logger::trace("found and insert {} {} {:x} not hair {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                            }
                        }
                    }
                }
            }
        });
        logger::trace("Init and build FormMap finished");
        return true;
    }

    void HeadPartExplorer::GetHeadPartConfig()
    {
        logger::info("Get Exception Head Parts...");
        std::string list = Config::GetSingleton().GetSetting().GetFeature().GetExceptionHeadParts();

        MultipleConfig::skipComments(list);
        MultipleConfig::trim(list);

        std::vector<std::string> splitlist = MultipleConfig::split(list, '|');

        for (std::size_t index = 0; index < splitlist.size(); index++)
        {
            logger::trace("Set {} on Exception Head Part", splitlist[index]);
            if (splitlist[index] == "EyeBrows")
                EyeBrows = true;
            else if (splitlist[index] == "Eyes")
                Eyes = true;
            else if (splitlist[index] == "Face")
                Face = true;
            else if (splitlist[index] == "FacialHair")
                FacialHair = true;
            else if (splitlist[index] == "Scar")
                Scar = true;
            else if (splitlist[index] == "Misc")
                Misc = true;
        }
    }
}