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

		bool RegisterActor(RE::Actor* actor, ControllerConfig config);
		bool RegisterPlayer();

		void ChangerActorState(RE::Actor* actor);
		void ChangerPlayerState();

		void TrackingActors(RE::Actor* actor);
		void InsertTrackingMap(std::vector<RE::FormID> baseid_list, std::string pluginname, ControllerConfig config);

	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		bool IsInitConfig = true;
		bool ElinAnimation = true;
		bool EnablePlayer = true;
		bool EnableNPCs = true;

		concurrency::concurrent_unordered_map<RE::FormID, AnimationController> ActorMap;

		const std::uint64_t FrameIntervalTimeTick_SE = 523660;
		const std::uint64_t FrameIntervalTimeTick_AE = 410199;
		float GetFrameIntervalTimeTick() { return *(float*)REL::RelocationID(FrameIntervalTimeTick_SE, FrameIntervalTimeTick_AE).address(); };

		void ControllActors();

		concurrency::concurrent_unordered_map<RE::FormID, ControllerConfig> TrackingMap;
	};
}
