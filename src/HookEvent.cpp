#include "HookEvent.h"

namespace Mus {
	void EventHandler::Register()
	{
		if (const auto Menu = RE::UI::GetSingleton()) {
			logger::info("Sinking menu events...");
			Menu->AddEventSink<RE::MenuOpenCloseEvent>(this);
		}

		if (const auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
			logger::info("Sinking loaded events...");
			EventHolder->AddEventSink<RE::TESLoadGameEvent>(this);
			EventHolder->AddEventSink<RE::TESObjectLoadedEvent>(this);
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
		}
	};

	EventResult EventHandler::ProcessEvent(const RE::TESLoadGameEvent* evn, RE::BSTEventSource<RE::TESLoadGameEvent>*) 
	{
		logger::trace("Detected Load Game Event");
		IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElin());
		if (!ActorManager::GetSingleton().RegisterPlayer())
		{
			logger::error("Failed register player on ElinAnimation");
		}
		else
		{
			ActorManager::GetSingleton().ChangerPlayerState();
		}
		RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();
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
}
