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
                RE::BGSHeadPart* data = static_cast<RE::BGSHeadPart*>(items[index]);

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
                        else if (Config::GetSingleton().GetRaceController() && Config::GetSingleton().GetBeforeSaveCompatible())
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
        std::vector<std::string> list = Config::GetSingleton().GetExceptionHeadParts();

        for (std::size_t index = 0; index < list.size(); index++)
        {
            logger::trace("Set {} on Exception Head Part", list[index]);
            if (list[index] == "EyeBrows")
                EyeBrows = true;
            else if (list[index] == "Eyes")
                Eyes = true;
            else if (list[index] == "Face")
                Face = true;
            else if (list[index] == "FacialHair")
                FacialHair = true;
            else if (list[index] == "Scar")
                Scar = true;
            else if (list[index] == "Misc")
                Misc = true;
        }
    }
}
