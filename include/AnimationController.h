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

	class AnimationController {
		RE::FormID id;
		std::string_view plugin;

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

		RE::Actor* elin;

		RE::NiNode* EarL11;
		RE::NiNode* EarR11;
		RE::NiNode* EarL12;
		RE::NiNode* EarR12;
		RE::NiNode* EarL13;
		RE::NiNode* EarR13;

		RE::NiNode* EarL21;
		RE::NiNode* EarR21;
		RE::NiNode* EarL22;
		RE::NiNode* EarR22;
		RE::NiNode* EarL23;
		RE::NiNode* EarR23;

		RE::NiNode* EarL31;
		RE::NiNode* EarR31;
		RE::NiNode* EarL32;
		RE::NiNode* EarR32;

		RE::NiNode* TailBone1;
		RE::NiNode* TailBone2;
		RE::NiNode* TailBone3;

		RE::NiNode* TailSpine;

		bool isValidNode = true;

		void updateEarsAnimation();
		void updateTailsAnimation();

		bool assignEarsNodes();
		bool assignTailsNodes();

		RE::NiNode* GetVirtualNode(RE::BSFixedString nodeName);

		ControllerConfig animConfig;
	};
}
