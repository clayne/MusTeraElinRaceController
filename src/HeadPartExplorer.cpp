#include "HeadPartExplorer.h"

namespace Mus {
    bool HeadPartExplorer::InitFormList() {
        logger::trace("Init and building FormMap...");

        auto handler = RE::TESDataHandler::GetSingleton();
        if (!handler) {
            logger::error("Missing TESDataHandler!");
            return false;
        }
        logger::trace("Get data handler success");

        auto& items = handler->GetFormArray(RE::FormType::HeadPart);
        logger::trace("Get Formarray related headpart");

        concurrency::parallel_for(RE::BSTArrayBase::size_type(0), items.size(), [&](RE::BSTArrayBase::size_type index) {
            if (items[index]) 
            {
                RE::FormID formID = items[index]->formID;
                RE::BGSHeadPart* data = reinterpret_cast<RE::BGSHeadPart*>(items[index]);

                if (data) {
                    if (IsPlayerble(data->flags) && (data->validRaces && IsNotForCustomRaceOnly(data->validRaces->forms)))
                    {
                        if (IsHairPart(data->type))
                        {
                            HeadPartFormMap.insert(std::make_pair(formID, data));
                            logger::trace("found and insert {} {} {:x} hair {}on formmap", GetModNameByID(formID), data->formEditorID.c_str(), formID, (data->IsExtraPart() ? "" : "extra "));
                        }
                    }
                }
            }
        });
        logger::trace("Init and build FormMap finished");
        return true;
    }
}