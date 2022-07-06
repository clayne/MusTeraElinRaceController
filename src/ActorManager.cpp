#include "ActorManager.h"

namespace Mus {
	ActorManager::ActorManager()
	{

	}

	ActorManager::~ActorManager()
	{

	}

	void ActorManager::onEvent(const FrameEvent& e)
	{
		//logger::trace("work onevent");
		const auto menu = RE::UI::GetSingleton();
		if ((e.gamePaused || menu->numPausesGame > 0) || IsRaceSexMenu.load() || IsMainMenu.load())
			return;

		if (!IsValidTeraElinRace ||
			!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->loadedData || !RE::PlayerCharacter::GetSingleton()->loadedData->data3D)
			return;

		concurrency::parallel_invoke(
			[&] {
				ControllActors();
		},  [&] {
				TrackingActors();
		});
	}

	void ActorManager::ControllActors()
	{
		const float IntervalTimeTick = GetFrameIntervalTimeTick();
		concurrency::parallel_for_each(ActorMap.begin(), ActorMap.end(), [&](auto& a) {
			RE::Actor* actor = reinterpret_cast<RE::Actor*>(RE::TESForm::LookupByID(a.first));
			if (actor)
			{
				if (a.first == RE::PlayerCharacter::GetSingleton()->formID)
				{
					ChangerPlayerState();
				}
				else
				{
					ChangerActorState(actor);
				}

				logger::trace("update the Actor on Elin animation : {} {:x}", actor->GetDisplayFullName(), a.first);
				a.second.IntervalTimeTick = IntervalTimeTick;
				a.second.update(actor);
			}
		});
	}

	void ActorManager::TrackingActors()
	{
		AIProcessManager* aiprocess = AIProcessManager::GetSingleton();
		for (RE::BSTArrayBase::size_type index = 0; index < aiprocess->actorsHigh.size(); index++)
		{
			RE::NiPointer<RE::Actor> actorpointer = RE::Actor::LookupByHandle(aiprocess->actorsHigh[index]);
			if (!actorpointer)
				continue;

			RE::Actor* actor = actorpointer.get();
			if (!actor || !actor->loadedData || !actor->loadedData->data3D)
				continue;

			auto tm = TrackingMap.find(GetActorBaseFormID(actor));
			if (tm == TrackingMap.end())
				continue;

			if (RegisterActor(actor, tm->second))
				TrackingMap.erase(tm);
		}
	}

	bool ActorManager::RegisterActor(RE::Actor* actor, ControllerConfig config)
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D)
			return false;

		if (ActorMap.find(actor->formID) != ActorMap.end())
			return true;

		auto controller = AnimationController(actor, config);
		ActorMap.insert(std::make_pair(actor->formID, controller));
		logger::info("Registered the {} on Elin animation : {} {:x}", (actor->formID == 0x14) ? "Player" : "Actor", actor->GetDisplayFullName(), actor->formID);

		return true;
	}

	bool ActorManager::RegisterPlayer()
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();

		if (!Player || !Player->loadedData || !Player->loadedData->data3D || !Player->loadedData->data3D.get() || IsPlayerElin.load())
			return false;

		logger::info("Try to register the Player on Elin animation : {} {:x}", Player->GetDisplayFullName(), Player->formID);

		ControllerConfig config;

		config.AnimationSpeed = Config::GetSingleton().GetSetting().GetAnimation().GetAnimationSpeed();
		config.FrequencyMax = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMax();
		config.FrequencyMin = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMin();
		config.Reversed = Config::GetSingleton().GetSetting().GetAnimation().GetReversed();
		config.DialogueAnger = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueAnger();
		config.DialogueFear = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueFear();
		config.DialogueHappy = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueHappy();
		config.DialogueSad = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueSad();
		config.DialogueSurprise = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueSurprise();
		config.DialoguePuzzled = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialoguePuzzled();
		config.DialogueDisgust = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueDisgust();
		config.MoodAnger = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodAnger();
		config.MoodFear = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodFear();
		config.MoodHappy = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodHappy();
		config.MoodSad = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodSad();
		config.MoodSurprise = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodSurprise();
		config.MoodPuzzled = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodPuzzled();
		config.MoodDisgust = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodDisgust();

		return RegisterActor(Player, config);
	}

	void ActorManager::ChangerActorState(RE::Actor* actor)
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D)
			return;

		auto am = ActorMap.find(actor->formID);
		if (am == ActorMap.end())
			return;

		AnimationController& ac = am->second;

		if (actor->IsDead())
			ac.IsValidActor = false;
	}

	void ActorManager::ChangerPlayerState()
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;
		
		auto am = ActorMap.find(Player->formID);
		if (am == ActorMap.end())
			return;

		AnimationController& ac = am->second;
		ac.IsValidActor = IsPlayerElin;
	}
}