#pragma once

namespace Mus {
	constexpr std::uintptr_t AIProcessManager_SE = 514167;
	constexpr std::uintptr_t AIProcessManager_AE = 400315;

	class AIProcessManager
	{
	public:
		static AIProcessManager* GetSingleton() {
			static REL::Relocation<RE::NiPointer<AIProcessManager>*> singleton{ REL::RelocationID(AIProcessManager_SE, AIProcessManager_AE) };
			return singleton->get();
		};

		void StopArtObject(RE::TESObjectREFR* ref, RE::BGSArtObject* art);

		std::uint8_t  unk000;                   // 008
		bool   enableDetection;          // 001 
		bool   unk002;                   // 002 
		std::uint8_t  unk003;                   // 003
		std::uint32_t unk004;                   // 004
		bool   enableHighProcess;        // 008 
		bool   enableLowProcess;         // 009 
		bool   enableMiddleHighProcess;  // 00A 
		bool   enableMiddleLowProcess;   // 00B 
		bool   enableAISchedules;        // 00C 
		std::uint8_t  unk00D;                   // 00D
		std::uint8_t  unk00E;                   // 00E
		std::uint8_t  unk00F;                   // 00F
		std::int32_t  numActorsInHighProcess;   // 010
		std::uint32_t unk014[(0x30 - 0x014) / sizeof(std::uint32_t)];
		RE::BSTArray<std::uint32_t>  actorsHigh; // 030 
		RE::BSTArray<std::uint32_t>  actorsLow;  // 048 
		RE::BSTArray<std::uint32_t>  actorsMiddleLow; // 060
		RE::BSTArray<std::uint32_t>  actorsMiddleHigh; // 078
		std::uint32_t  unk90[(0xF0 - 0x7C) / sizeof(std::uint32_t)];
		RE::BSTArray<void*> activeEffectShaders; // 108
										   //mutable BSUniqueLock			 activeEffectShadersLock; // 120
	};
	static_assert(offsetof(AIProcessManager, numActorsInHighProcess) >= 0x10, "Unk141F831B0::actorsHigh is too early!");
	static_assert(offsetof(AIProcessManager, numActorsInHighProcess) <= 0x10, "Unk141F831B0::actorsHigh is too late!");

	static_assert(offsetof(AIProcessManager, actorsHigh) >= 0x030, "Unk141F831B0::actorsHigh is too early!");
	static_assert(offsetof(AIProcessManager, actorsHigh) <= 0x039, "Unk141F831B0::actorsHigh is too late!");

	static_assert(offsetof(AIProcessManager, actorsLow) >= 0x048, "Unk141F831B0::actorsLow is too early!");
	static_assert(offsetof(AIProcessManager, actorsLow) <= 0x048, "Unk141F831B0::actorsLow is too late!");

	static_assert(offsetof(AIProcessManager, actorsMiddleLow) >= 0x060, "Unk141F831B0::actorsMiddleLow is too early!");
	static_assert(offsetof(AIProcessManager, actorsMiddleLow) <= 0x060, "Unk141F831B0::actorsMiddleLow is too late!");

	static_assert(offsetof(AIProcessManager, actorsMiddleHigh) >= 0x078, "Unk141F831B0::actorsMiddleHigh is too early!");
	static_assert(offsetof(AIProcessManager, actorsMiddleHigh) <= 0x078, "Unk141F831B0::actorsMiddleHigh is too late!");

	static_assert(offsetof(AIProcessManager, activeEffectShaders) >= 0x108, "Unk141F831B0::activeEffectShaders is too early!");
	static_assert(offsetof(AIProcessManager, activeEffectShaders) <= 0x108, "Unk141F831B0::activeEffectShaders is too late!");



	class ActorManager
		: public IEventListener<FrameEvent>
	{
	public:
		ActorManager();
		~ActorManager();

		[[nodiscard]] static ActorManager* instance() {
			static ActorManager instance;
			return &instance;
		}

		bool RegisterActor(RE::Actor* actor, ControllerConfig config);
		bool RegisterPlayer();

		void ChangerActorState(RE::Actor* actor);
		void ChangerPlayerState();

		std::unordered_map<RE::FormID, ControllerConfig> TrackingMap;

	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		bool IsInitConfig = true;
		bool ElinAnimation = true;
		bool EnablePlayer = true;
		bool EnableNPCs = true;

		concurrency::concurrent_unordered_map<RE::FormID, AnimationController> ActorMap;

		const std::uintptr_t FrameIntervalTimeTick_SE = 523660;
		const std::uintptr_t FrameIntervalTimeTick_AE = 410199;
		float GetFrameIntervalTimeTick() { return *(float*)REL::RelocationID(FrameIntervalTimeTick_SE, FrameIntervalTimeTick_AE).address(); };

		void TrackingActors();
		void ControllActors();
	};
}
