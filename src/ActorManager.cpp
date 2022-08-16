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
		//logger::error("work onevent");
		const auto menu = RE::UI::GetSingleton();
		if (((e.gamePaused || (menu && menu->numPausesGame > 0)) && !IsRaceSexMenu.load()) || IsMainMenu.load())
			return;

		if (!IsValidTeraElinRace ||
			!RE::PlayerCharacter::GetSingleton() || !RE::PlayerCharacter::GetSingleton()->loadedData || !RE::PlayerCharacter::GetSingleton()->loadedData->data3D)
			return;

		//ControllAnimtionActors();
		ControllFacegenActors();
	}

	void ActorManager::ControllAnimtionActors()
	{
		const float IntervalTimeTick = GetFrameIntervalTimeTick();
		concurrency::parallel_for_each(ActorMap.begin(), ActorMap.end(), [&](auto& map) {
			RE::Actor* actor = static_cast<RE::Actor*>(RE::TESForm::LookupByID(map.first));
			if (actor)
			{
				if (isPlayer(actor->formID))
				{
					ChangerPlayerState(map.second);
				}
				else
				{
					ChangerActorState(map.second, actor);
				}

				logger::trace("update the Actor on Elin animation : {} {:x}", actor->GetDisplayFullName(), map.first);
				map.second.IntervalTimeTick = IntervalTimeTick;
				map.second.update(actor);
			}
		});
	}

	void ActorManager::ControllFacegenActors()
	{
		concurrency::parallel_for_each(FaceGenDataMap.begin(), FaceGenDataMap.end(), [&](auto& map) {
			RE::Actor* actor = static_cast<RE::Actor*>(RE::TESForm::LookupByID(map.first));
			if (actor)
			{
				if (isPlayer(actor->formID))
				{
					ChangerPlayerState(map.second);
				}
				else
				{
					ChangerActorState(map.second, actor);
				}

				logger::trace("update the Actor on Elin animation : {} {:x}", actor->GetDisplayFullName(), map.first);
				map.second.update(actor);
			}
		});
	}

	void ActorManager::TrackingActors(RE::Actor* actor)
	{
		if (!actor)
			return;
		
		logger::debug("Verify configure the actor {} {:x}...", actor->GetDisplayFullName(), actor->formID);

		RE::TESNPC* npc = actor->GetActorBase();
		
		if (!npc->GetRace()->GetFormEditorID())
			return;
		std::string_view racenameview = npc->GetRace()->GetFormEditorID();
		logger::info("Checking Race {} {} {:x}...", racenameview.data(), actor->GetDisplayFullName(), actor->formID);
		std::string racename = lowLetter(racenameview);
		if (racename.find("elin") == std::string::npos)
		{
			logger::info("{} {} {:x} is not support race", racenameview.data(), actor->GetDisplayFullName(), actor->formID);
			return;
		}

		auto list = TrackingMap.find(npc->formID);

		ControllerConfig config;

		if (list == TrackingMap.end())
			config = MultipleConfig::GetConfigDefault();
		else
			config = list->second;

		RegisterAnimationActor(actor, config);
		RegisterFacegenActor(actor, config);
	}

	bool ActorManager::RegisterAnimationActor(RE::Actor* actor, ControllerConfig config)
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D)
			return false;

		if (ActorMap.find(actor->formID) != ActorMap.end())
			return true;

		if (!Config::GetSingleton().GetElinAnimation())
			return false;
		if ((isPlayer(actor->formID)) && !Config::GetSingleton().GetEnablePlayer())
			return false;
		else if (!isPlayer(actor->formID) && !Config::GetSingleton().GetEnableNPCs())
			return false;
		auto controller = AnimationController(actor, config);
		ActorMap.insert(std::make_pair(actor->formID, controller));
		logger::info("Registered the {} on Elin animation : {} {:x}", (actor->formID == 0x14) ? "Player" : "Actor", actor->GetDisplayFullName(), actor->formID);

		return true;
	}

	bool ActorManager::RegisterFacegenActor(RE::Actor* actor, ControllerConfig config)
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D)
			return false;

		if (FaceGenDataMap.find(actor->formID) != FaceGenDataMap.end())
			return true;
		
		if (!Config::GetSingleton().GetEmotionHeadOverlay())
			return false;
		if (!isPlayer(actor->formID) && Config::GetSingleton().GetEmotionHeadOverlayOnlyPlayer())
			return false;
		auto facegendetector = FaceGenMorphDetector(actor, config.EmotionEffectActiveThreshold);
		FaceGenDataMap.insert(std::make_pair(actor->formID, facegendetector));
		logger::info("Registered the {} on Elin facegen : {} {:x}", (actor->formID == 0x14) ? "Player" : "Actor", actor->GetDisplayFullName(), actor->formID);

		return true;
	}

	bool ActorManager::RegisterPlayer()
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();

		if (!Player || !Player->loadedData || !Player->loadedData->data3D || !Player->loadedData->data3D.get() || !IsPlayerElin.load())
			return false;

		logger::info("Try to register the Player on Elin animation : {} {:x}", Player->GetDisplayFullName(), Player->formID);

		ControllerConfig config = MultipleConfig::GetConfigDefault();

		return (RegisterAnimationActor(Player, config) && RegisterFacegenActor(Player, config));
	}

	void ActorManager::ChangerActorState(AnimationController& ac, RE::Actor* actor)
	{
		if (!actor)
			return;

		if (actor->IsDead() || actor->IsDeleted())
			ac.IsValidActor = false;
		else if (!actor->loadedData || !actor->loadedData->data3D)
			ac.IsValidActor = false;
		else
			ac.IsValidActor = true;
	}

	void ActorManager::ChangerActorState(FaceGenMorphDetector& fgd, RE::Actor* actor)
	{
		if (!actor)
			return;

		if (actor->IsDead() || actor->IsDeleted())
			fgd.IsValidActor = false;
		else if (!actor->loadedData || !actor->loadedData->data3D)
			fgd.IsValidActor = false;
		else
			fgd.IsValidActor = true;
	}

	void ActorManager::ChangerPlayerState(AnimationController& ac)
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		ac.IsValidActor = IsPlayerElin;

		if (!Player->loadedData || !Player->loadedData->data3D)
			ac.IsValidActor = false;
	}

	void ActorManager::ChangerPlayerState(FaceGenMorphDetector& fgd)
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		fgd.IsValidActor = IsPlayerElin;

		if (!Player->loadedData || !Player->loadedData->data3D)
			fgd.IsValidActor = false;
	}

	void ActorManager::InsertTrackingMap(std::vector<RE::FormID> baseid_list, std::string pluginname, ControllerConfig config)
	{
		for (size_t index = 0; index < baseid_list.size(); index++)
		{
			logger::debug("Verify the baseid is valid NPC : {} {:x}", pluginname, baseid_list.at(index));
			RE::TESForm* obj = GetFormByID(baseid_list.at(index), pluginname);
			if (!obj || obj->IsNot(RE::FormType::NPC))
				continue;

			TrackingMap.insert(std::make_pair(baseid_list.at(index), config));
			logger::info("insert {} {:x} on tracking list", pluginname, baseid_list.at(index));
		}
	}

	void ActorManager::UpdatePlayerFaceNodes()
	{
		auto* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto datamap = FaceGenDataMap.find(Player->formID);
		if (datamap == FaceGenDataMap.end())
		{
			logger::trace("Cannot use FaceGen DataMap : {} {:x}", Player->GetDisplayFullName(), Player->formID);
		}
		else
		{
			FaceGenMorphDetector& facegendetector = datamap->second;
			facegendetector.PlayerReset(Player);
		}
	}
}
