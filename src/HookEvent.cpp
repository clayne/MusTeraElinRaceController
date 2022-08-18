#include "HookEvent.h"

namespace Mus {
	void EventHandler::Register()
	{
		if (const auto Menu = RE::UI::GetSingleton()) {
			logger::info("Sinking menu events...");
			Menu->AddEventSink<RE::MenuOpenCloseEvent>(this);
		}

		if (const auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
			logger::info("Sinking load/switch events...");
			EventHolder->AddEventSink<RE::TESLoadGameEvent>(this);
			EventHolder->AddEventSink<RE::TESObjectLoadedEvent>(this);
			EventHolder->AddEventSink<RE::TESSwitchRaceCompleteEvent>(this);
			EventHolder->AddEventSink<RE::TESMagicEffectApplyEvent>(this);
		}
	}

	EventResult EventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!evn || !evn->menuName.c_str())
			return EventResult::kContinue;

		if (evn->opening)
			MenuOpened(evn->menuName.c_str());
		else
			MenuClosed(evn->menuName.c_str());

		return EventResult::kContinue;
	}

	void EventHandler::MenuOpened(std::string name)
	{
		if (name == "Main Menu")
		{
			logger::trace("Detected Main Menu");
			IsMainMenu.store(true);
		}
		else if (name == "RaceSex Menu")
		{
			logger::trace("Detected RaceSex Menu");
			IsRaceSexMenu.store(true);
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElinHumanoidVampireLord());
			//g_frameEventDispatcher.addListener(&PlayerGenderDetector::GetSingleton());
			//RaceSexMenuTracker::GetSingleton().DebugSliderPrint();
			//RaceSexMenuTracker::GetSingleton().InitTrackingList();
			//g_frameEventDispatcher.addListener(&RaceSexMenuTracker::GetSingleton());
		}
	}

	void EventHandler::MenuClosed(std::string name)
	{
		if (name == "Main Menu")
		{
			IsMainMenu.store(false);
		}
		else if (name == "RaceSex Menu")
		{
			IsRaceSexMenu.store(false);
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElinHumanoidVampireLord());
			ActorManager::GetSingleton().UpdatePlayerFaceNodes();
			//g_frameEventDispatcher.removeListener(&RaceSexMenuTracker::GetSingleton());
		}
	}

	EventResult EventHandler::ProcessEvent(const RE::TESLoadGameEvent* evn, RE::BSTEventSource<RE::TESLoadGameEvent>*) 
	{
		logger::debug("Detected Load Game Event");
		InputEventHandler::GetSingleton()->Register();
		RaceCompatibility::GetSingleton().LoadRaceCompatibility();
		RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();
		//RaceCompatibility::GetSingleton().SolveCompatibleVampireLord();
		if (!ActorManager::GetSingleton().RegisterPlayer())
		{
			logger::error("Failed register player on ElinAnimation");
		}

		return EventResult::kContinue;
	}

	EventResult EventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* evn, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
	{
		if (!evn)
			return EventResult::kContinue;	
		
		RE::TESForm* obj = RE::TESForm::LookupByID(evn->formID);
		if (!obj || obj->IsNot(RE::FormType::ActorCharacter))
			return EventResult::kContinue;

		RE::Actor* actor = static_cast<RE::Actor*>(obj);

		if (!actor)
			return EventResult::kContinue;

		if (isPlayer(actor->formID))
			return EventResult::kContinue;

		auto& am = ActorManager::GetSingleton();
		am.TrackingActors(actor);

		return EventResult::kContinue;
	}

	EventResult EventHandler::ProcessEvent(const RE::TESSwitchRaceCompleteEvent* evn, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*)
	{
		if (!evn || !evn->subject)
			return EventResult::kContinue;

		auto* npc = evn->subject.get();
		if (!npc)
			return EventResult::kContinue;

		//logger::error("Detected Race change Event : {:x}", npc->formID);

		if (npc->formID == 0x7 || npc->formID == 0x14)
		{
			logger::debug("Detected Player Race change");
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElinHumanoidVampireLord());

			if (IsPlayerElin.load())
			{
				RaceCompatibility::GetSingleton().LoadRaceCompatibility();
				RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();
				//RaceCompatibility::GetSingleton().SolveCompatibleVampireLord();
				if (!ActorManager::GetSingleton().RegisterPlayer())
				{
					logger::error("Failed register player on ElinAnimation");
				}

				ActorManager::GetSingleton().UpdatePlayerFaceNodes();
			}
		}
		return EventResult::kContinue;
	}

	EventResult EventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* evn, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*)
	{
		if (!evn || !evn->caster || !evn->caster.get())
			return EventResult::kContinue;

		auto* npc = evn->caster.get();

		if (npc && isPlayer(npc->formID) && IsPlayerElin.load())
		{
			if (evn->magicEffect == VampireLordMagicEffectID)
				RaceCompatibility::GetSingleton().SolveCompatibleVampireLord();
			else if (evn->magicEffect == RevertVampireLordMagicEffectID)
				RaceCompatibility::GetSingleton().RevertCompatibleVampireLord();
		}
		return EventResult::kContinue;
	}

	
	void InputEventHandler::Register()
	{
		/*if (const auto Device = RE::BSInputDeviceManager::GetSingleton()) {
			logger::info("Sinking device events...");
			Device->AddEventSink(this);
		}*/
	}

	EventResult InputEventHandler::ProcessEvent(const RE::InputEvent*& evn, RE::BSTEventSource<RE::InputEvent*>*) {
		if (isFirstRun)
		{
			EarLKey = Config::GetSingleton().GetLEarHotkey();
			EarRKey = Config::GetSingleton().GetREarHotkey();
			TailKey = Config::GetSingleton().GetTailHotkey();
			isFirstRun = false;
		}

		logger::debug("InputEvent");

		if (!evn)
			return EventResult::kContinue;

		const RE::ButtonEvent* button = evn->AsButtonEvent();
		if (!button)
			return EventResult::kContinue;

		if (button->IsPressed())
		{
			if (button->GetIDCode() == EarLKey)
			{
				PressEarLKey.store(true);
			}
			else if (button->GetIDCode() == EarRKey)
			{
				PressEarRKey.store(true);
			}
			else if (button->GetIDCode() == TailKey)
			{
				PressTailKey.store(true);
			}

			logger::debug("Press Button Event : {}", button->GetIDCode());
		}
		else
		{
			if (button->GetIDCode() == EarLKey)
			{
				PressEarLKey.store(false);
			}
			else if (button->GetIDCode() == EarRKey)
			{
				PressEarRKey.store(false);
			}
			else if (button->GetIDCode() == TailKey)
			{
				PressTailKey.store(false);
			}

			logger::debug("Unpress Button Event : {}", button->GetIDCode());
		}
		return EventResult::kContinue;
	}
}
