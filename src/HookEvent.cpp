#include "HookEvent.h"

namespace Mus {
	void EventHandler::Register()
	{
		if (const auto Menu = RE::UI::GetSingleton()) {
			logger::info("Sinking menu events...");
			Menu->AddEventSink<RE::MenuOpenCloseEvent>(this);
		}

		if (const auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton()) {
			logger::info("Sinking game load events...");
			EventHolder->AddEventSink<RE::TESLoadGameEvent>(this);
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
			logger::info("Detected Main Menu");
			IsMainMenu.store(true);
		}
		else if (name == "RaceSex Menu")
		{
			logger::info("Detected RaceSex Menu");
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
		if (!ActorManager::instance()->RegisterPlayer())
		{
			logger::error("Failed register player on ElinAnimation");
			return EventResult::kContinue;
		}
		ActorManager::instance()->ChangerPlayerState();

		return EventResult::kContinue;
	}
}
