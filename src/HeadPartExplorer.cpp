#include "HeadPartExplorer.h"

namespace Mus {
    bool HeadPartExplorer::InitFormList() {
        logger::trace("Init and building FormMap...");

        auto handler = RE::TESDataHandler::GetSingleton();
        if (!handler) {
            logger::error("Missing TESDataHandler!");
            return false;
        }

        auto& items = handler->GetFormArray(RE::FormType::HeadPart);
        logger::trace("Get Formarray related headpart");

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
                        if (IsHairPart(data->type))
                        {
                            HeadPartFormMap.insert(std::make_pair(formID, data));
                            logger::trace("found and insert {} {} {:x} hair {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                        }
                        else if (Config::GetSingleton().GetSetting().GetFeature().GetRaceController() && Config::GetSingleton().GetSetting().GetFeature().GetBeforeSaveCompatible())
                        {
                            OtherHeadPartFormMap.insert(std::make_pair(formID, data));
                            logger::trace("found and insert {} {} {:x} not hair {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "extra " : ""));
                        }
                    }
                }
            }
        });
        logger::trace("Init and build FormMap finished");
        return true;
    }
}