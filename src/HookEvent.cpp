#include "HookEvent.h"

namespace Mus {
	void EventHandler::Register(bool isLoaded)
	{
		if (!isLoaded)
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
			}
		}
		else
		{
			if (const auto Device = RE::BSInputDeviceManager::GetSingleton()) {
				logger::info("Sinking device events...");
				Device->AddEventSink(InputEventHandler::GetSingleton());
			}
		}
	}

	EventResult EventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!evn)
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
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElin());
			g_frameEventDispatcher.addListener(&PlayerGenderDetector::GetSingleton());
		}
	};

	void EventHandler::MenuClosed(std::string name)
	{
		if (name == "Main Menu")
		{
			IsMainMenu.store(false);
		}
		else if (name == "RaceSex Menu")
		{
			IsRaceSexMenu.store(false);
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElin());
			g_frameEventDispatcher.removeListener(&PlayerGenderDetector::GetSingleton());
			ActorManager::GetSingleton().UpdatePlayerFaceNodes();
		}
	};

	EventResult EventHandler::ProcessEvent(const RE::TESLoadGameEvent* evn, RE::BSTEventSource<RE::TESLoadGameEvent>*) 
	{
		logger::debug("Detected Load Game Event");
		Register(true);
		RaceCompatibility::GetSingleton().LoadRaceCompatibility();
		RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();
		RaceCompatibility::GetSingleton().SolveCompatibleVampireLord();

		if (!ActorManager::GetSingleton().RegisterPlayer())
		{
			logger::error("Failed register player on ElinAnimation");
		}
		else
		{
			ActorManager::GetSingleton().ChangerPlayerState();
		}

		return EventResult::kContinue;
	}

	EventResult EventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* evn, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
	{
		if (!evn)
			return EventResult::kContinue;	
		
		if (!evn->loaded)
			return EventResult::kContinue;
			
		RE::TESForm* obj = RE::TESForm::LookupByID(evn->formID);
		if (!obj || obj->IsNot(RE::FormType::ActorCharacter))
			return EventResult::kContinue;

		RE::Actor* actor = reinterpret_cast<RE::Actor*>(obj);

		if (!actor)
			return EventResult::kContinue;

		static auto& am = ActorManager::GetSingleton();
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

		logger::error("Detected Race change Event : {:x}", npc->formID);

		if (npc->formID == 0x7 || npc->formID == 0x14)
		{
			logger::debug("Detected Player Race change");
			IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElin());

			if (IsPlayerElin.load())
			{
				RaceCompatibility::GetSingleton().LoadRaceCompatibility();
				RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();
				RaceCompatibility::GetSingleton().SolveCompatibleVampireLord();

				if (!ActorManager::GetSingleton().RegisterPlayer())
				{
					logger::error("Failed register player on ElinAnimation");
				}
				else
				{
					ActorManager::GetSingleton().ChangerPlayerState();
				}
			}
		}

		return EventResult::kContinue;
	}
	/*
	EventResult EventHandler::ProcessEvent(const RE::InputEvent* evn, RE::BSTEventSource<RE::InputEvent>*)
	{
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

			logger::trace("Press Button Event : {}", button->GetIDCode());
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

			logger::trace("Unpress Button Event : {}", button->GetIDCode());
		}

		return EventResult::kContinue;
	}
	*/
}
