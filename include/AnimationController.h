#pragma once

namespace Mus {
	enum animation_type
	{
		noAnim,
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
		std::uint8_t AnimationSpeed = 3;
		bool Reversed = false;
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

	struct ElinNodes {
		RE::NiAVObject* EarL11;
		RE::NiAVObject* EarR11;
		RE::NiAVObject* EarL12;
		RE::NiAVObject* EarR12;
		RE::NiAVObject* EarL13;
		RE::NiAVObject* EarR13;

		RE::NiAVObject* EarL21;
		RE::NiAVObject* EarR21;
		RE::NiAVObject* EarL22;
		RE::NiAVObject* EarR22;
		RE::NiAVObject* EarL23;
		RE::NiAVObject* EarR23;

		RE::NiAVObject* EarL31;
		RE::NiAVObject* EarR31;
		RE::NiAVObject* EarL32;
		RE::NiAVObject* EarR32;

		RE::NiAVObject* TailBone1;
		RE::NiAVObject* TailBone2;
		RE::NiAVObject* TailBone3;

		RE::NiAVObject* TailSpine;
	};

	class AnimationController{
		RE::Actor* elin;
		RE::FormID id;
		std::string name;
		ControllerConfig animConfig;

	public:
		bool IsValidActor = true;
		float IntervalTimeTick = 1.0f / 60.0f;

		AnimationController(RE::Actor* actor, ControllerConfig config);
		~AnimationController();
		void update(RE::Actor* actor);

	private:
		void isCannotReadNode(bool isEnd);
		std::vector<const char*> VNodeEntries;
		int isCannotReadCount = -1;
		bool isCountUpdated = false;

		ElinNodes en;

		bool isValidNode = true;

		void updateEarsAnimation();
		void updateTailAnimation();

		void resetEarsAnimation();
		void resetTailAnimation();

		bool assignEarsNodes();
		bool assignTailNodes();

		void updateEarsNodes();
		void updateTailNodes();

		void updateNode(RE::NiAVObject* node);

		RE::NiAVObject* GetVirtualNode(RE::BSFixedString nodeName);

		float test = 0.0f;
	};
}
