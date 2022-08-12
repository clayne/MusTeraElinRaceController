#pragma once

namespace Mus {
	class ActorManager
		: public IEventListener<FrameEvent>
	{
	public:
		ActorManager();
		~ActorManager();

		[[nodiscard]] static ActorManager& GetSingleton() {
			static ActorManager instance;
			return instance;
		}

		bool RegisterAnimationActor(RE::Actor* actor, ControllerConfig config);
		bool RegisterFacegenActor(RE::Actor* actor, ControllerConfig config);
		bool RegisterPlayer();

		void ChangerActorState(AnimationController& ac, RE::Actor* actor);
		void ChangerActorState(FaceGenMorphDetector& fgd, RE::Actor* actor);
		void ChangerPlayerState(AnimationController& ac);
		void ChangerPlayerState(FaceGenMorphDetector& fgd);

		void TrackingActors(RE::Actor* actor);
		void InsertTrackingMap(std::vector<RE::FormID> baseid_list, std::string pluginname, ControllerConfig config);

		inline FaceGenMorphDetector* GetFaceGenData(RE::FormID id) {
			auto data = FaceGenDataMap.find(id);
			if (data == FaceGenDataMap.end())
				return nullptr;

			return &data->second;
		};

		void UpdatePlayerFaceNodes();
	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		concurrency::concurrent_unordered_map<RE::FormID, AnimationController> ActorMap;
		concurrency::concurrent_unordered_map<RE::FormID, FaceGenMorphDetector> FaceGenDataMap;

		const std::uint64_t FrameIntervalTimeTick_SE = 523660;
		const std::uint64_t FrameIntervalTimeTick_AE = 410199;
		const float TimeTick60 = 1.0f / 60.0f;
		float GetFrameIntervalTimeTick() { return (*(float*)REL::RelocationID(FrameIntervalTimeTick_SE, FrameIntervalTimeTick_AE).address()) / TimeTick60; };

		void ControllAnimtionActors();
		void ControllFacegenActors();

		concurrency::concurrent_unordered_map<RE::FormID, ControllerConfig> TrackingMap;
	};
}
