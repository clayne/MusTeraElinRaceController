#pragma once

namespace Mus {
	enum class animation_type : std::uint8_t
	{
		noAnim = -1,
		continueAnim,
		continueAnimL,
		continueAnimR,
		droop,
		droopL,
		droopR,
		attention,
		attentionL,
		attentionR
	};

	struct ControllerConfig {
		float FrequencyMax = 7.5;
		float FrequencyMin = 2.5;
		std::uint8_t AnimationEarsSpeed = 3;
		std::uint8_t AnimationTailSpeed = 3;
		bool Reversed = false;
		std::uint8_t EmotionEffectActiveThreshold = 70;
		std::uint8_t EmotionAnimationActiveThreshold = 70;
		animation_type DialogueAnger = animation_type::attention;
		animation_type DialogueFear = animation_type::droop;
		animation_type DialogueHappy = animation_type::continueAnim;
		animation_type DialogueSad = animation_type::droop;
		animation_type DialogueSurprise = animation_type::attention;
		animation_type DialoguePuzzled = animation_type::continueAnimL;
		animation_type DialogueDisgust = animation_type::droop;
		animation_type MoodAnger = animation_type::attention;
		animation_type MoodFear = animation_type::droop;
		animation_type MoodHappy = animation_type::continueAnim;
		animation_type MoodSad = animation_type::droop;
		animation_type MoodSurprise = animation_type::attention;
		animation_type MoodPuzzled = animation_type::continueAnimL;
		animation_type MoodDisgust = animation_type::droop;
	};

	class AnimationController {
		RE::Actor* elin;
		RE::FormID id;
		std::string name;
		ControllerConfig animConfig;
		clock_t BeforeTime;

	public:
		bool IsValidActor = true;
		float IntervalTimeTick = 1.0f;

		AnimationController(RE::Actor* actor, ControllerConfig config);
		~AnimationController();
		void update(RE::Actor* actor);

	private:
		void isCannotReadNode(bool isEnd);
		std::vector<const char*> VNodeEntries;
		int isCannotReadCount = -1;
		bool isCountUpdated = false;

		struct ElinNodes {
			RE::NiAVObject* EarL11;
			RE::NiAVObject* EarR11;

			RE::NiAVObject* EarL21;
			RE::NiAVObject* EarR21;

			RE::NiAVObject* EarL31;
			RE::NiAVObject* EarR31;

			RE::NiAVObject* TailBone1;

			RE::NiAVObject* TailSpine;
		};

		ElinNodes en;

		bool isValidNode = true;

		enum class condition_type
		{
			nothing = -1,
			button,
			facemorph,
			random
		};

		RE::NiPoint3 LEarRot = RE::NiPoint3(0, 0, 0);
		RE::NiPoint3 REarRot = RE::NiPoint3(0, 0, 0);
		RE::NiPoint3 TailRot = RE::NiPoint3(0, 0, 0);

		RE::NiPoint3 LDEarRot = RE::NiPoint3(0, 0, 0); //dog ear
		RE::NiPoint3 RDEarRot = RE::NiPoint3(0, 0, 0); //dog ear

		const double toDegree = std::numbers::pi / 180;

		const RE::NiPoint3 EarLLimitNormal = RE::NiPoint3(-40.0f, 20.0f, -30.0f) * (toDegree);
		const RE::NiPoint3 EarRLimitNormal = RE::NiPoint3(40.0f, -20.0f, -30.0f) * (toDegree);
		const RE::NiPoint3 EarLUnitNormal = EarLLimitNormal * 0.01f;
		const RE::NiPoint3 EarRUnitNormal = EarRLimitNormal * 0.01f;

		const RE::NiPoint3 EarLDLimitNormal = RE::NiPoint3(0.0f, 0.0f, -40.0f) * (toDegree); //dog
		const RE::NiPoint3 EarRDLimitNormal = RE::NiPoint3(0.0f, 0.0f, -40.0f) * (toDegree); //dog
		const RE::NiPoint3 EarLDUnitNormal = EarLDLimitNormal * 0.01f;
		const RE::NiPoint3 EarRDUnitNormal = EarRDLimitNormal * 0.01f;

		const RE::NiPoint3 EarLLimitAttention = RE::NiPoint3(0.0f, 0.0f, -30.0f) * (toDegree);
		const RE::NiPoint3 EarRLimitAttention = RE::NiPoint3(0.0f, 0.0f, -30.0f) * (toDegree);
		const RE::NiPoint3 EarLUnitAttention = EarLLimitAttention * 0.01f;
		const RE::NiPoint3 EarRUnitAttention = EarRLimitAttention * 0.01f;

		const RE::NiPoint3 EarLDLimitAttention = RE::NiPoint3(-30.0f, 0.0f, 0.0f) * (toDegree); //dog
		const RE::NiPoint3 EarRDLimitAttention = RE::NiPoint3(-30.0f, 0.0f, 0.0f) * (toDegree); //dog
		const RE::NiPoint3 EarLDUnitAttention = EarLDLimitAttention * 0.01f;
		const RE::NiPoint3 EarRDUnitAttention = EarRDLimitAttention * 0.01f;

		const RE::NiPoint3 EarLLimitJitter = EarLLimitNormal/3;
		const RE::NiPoint3 EarRLimitJitter = EarLLimitNormal/3;
		const RE::NiPoint3 EarLUnitJitter = EarLLimitJitter * 0.01f * 3;
		const RE::NiPoint3 EarRUnitJitter = EarRLimitJitter * 0.01f * 3;

		const RE::NiPoint3 EarLDLimitJitter = EarLDLimitNormal; //dog
		const RE::NiPoint3 EarRDLimitJitter = EarRDLimitNormal; //dog
		const RE::NiPoint3 EarLDUnitJitter = EarLDLimitJitter * 0.01f;
		const RE::NiPoint3 EarRDUnitJitter = EarRDLimitJitter * 0.01f;

		const RE::NiPoint3 emptyPoint = RE::NiPoint3(0.0f, 0.0f, 0.0f);

		float velocityL = 1.0f;
		float velocityR = 1.0f;
		float velocity = 1.0f; //tail

		bool IsDoL = true;
		bool IsDoR = true;
		bool IsDo = true;

		bool IsDoing = false;

		enum class anim_state : std::uint8_t {
			Normal,
			Attention,
			Jitter
		};

		condition_type currentState = condition_type::nothing;

		condition_type IsCondition();

		void updateEarsAnimation(std::uint8_t type); //0 is button, 1 is morph, 2 is random, -1 is nothing
		void updateTailAnimation(std::uint8_t type); //0 is button, 1 is morph, 2 is random, -1 is nothing

		void DoEarsAnimation(std::uint8_t state);
		void DoEarsRandomAnimation();

		void DoEarsLAnimationNormal();
		void DoEarsRAnimationNormal();
		void DoEarsLAnimationAttention();
		void DoEarsRAnimationAttention();
		void DoEarsLAnimationJitter();
		void DoEarsRAnimationJitter();

		void DoTailAnimation();


		void UnDoEarsAnimation();
		void UnDoEarsRandomAnimation();

		void UnDoEarsLAnimationNormal();
		void UnDoEarsRAnimationNormal();
		void UnDoEarsLAnimationAttention();
		void UnDoEarsRAnimationAttention();
		void UnDoEarsLAnimationJitter();
		void UnDoEarsRAnimationJitter();

		void UnDoTailAnimation();

		void MorphEarsAnimation();
		void MorphTailAnimation();

		bool isRandomTime(bool isReset);
		clock_t RandomTime = 0;
		bool isRandomWait = false;
		enum class anim_state2 : std::uint8_t {
			NormalL,
			NormalR,
			Normal,
			AttentionL,
			AttentionR,
			Attention,
			JitterL,
			JitterR,
			Jitter,

			Total
		};
		anim_state2 RandomAnimType = anim_state2::Total;
		anim_state2 GetRandomType();

		bool isFaceMorph();
		animation_type getAnimationType(std::uint32_t expression);

		inline bool IsPressedButton() { return (PressEarLKey.load() || PressEarRKey.load() || PressTailKey.load()); };

		void resetEarsAnimation();
		void resetTailAnimation();

		bool assignEarsNodes();
		bool assignTailNodes();

		inline void setNodeRotation(RE::NiAVObject* node, RE::NiPoint3 RotPoint) {
			if (!node)
				return;
			RE::NiMatrix3 rot = RE::NiMatrix3(RotPoint);
			node->local.rotate = rot;
		};

		inline void clampNiPoint3(RE::NiPoint3& value, RE::NiPoint3 limit) {
			if (limit.x >= 0)
				value.x = std::clamp(value.x, 0.0f, limit.x);
			else
				value.x = std::clamp(value.x, limit.x, 0.0f);
			if (limit.y >= 0)
				value.y = std::clamp(value.y, 0.0f, limit.y);
			else
				value.y = std::clamp(value.y, limit.y, 0.0f);
			if (limit.z >= 0)
				value.z = std::clamp(value.z, 0.0f, limit.z);
			else
				value.z = std::clamp(value.z, limit.z, 0.0f);
		};

		inline bool compareNiPoint3(RE::NiPoint3 value, RE::NiPoint3 base) {
			return (IsApproximate(value.x, base.x) && IsApproximate(value.y, base.y) && IsApproximate(value.z, base.z));
		}

		inline bool IsApproximate(float value, float base) {
			return (((base + 0.001f) >= value) && ((base - 0.001f) <= value));
		}

		void updateEarsNodes();
		void updateTailNodes();

		void updateNode(RE::NiAVObject* node);

		RE::NiAVObject* GetVirtualNode(RE::BSFixedString nodeName);

		clock_t currentTime = 0;

		bool isResetRaceMenu = false;
	};
}
