#pragma once

namespace Mus
{
	class FaceGenMorphDetector {
		RE::Actor* elin;
		RE::FormID id;
		std::string name;

	public:
		enum class expression_type : std::uint32_t {
			dialogue_anger,
			dialogue_fear,
			dialogue_happy,
			dialogue_sad,
			dialogue_surprise,
			dialogue_puzzled,
			dialogue_disgust,
			mood_neutral,
			mood_anger,
			mood_fear,
			mood_happy,
			mood_sad,
			mood_surprise,
			mood_puzzled,
			mood_disgust,
			combat_anger,
			combat_shout,

			Total
		};

		FaceGenMorphDetector(RE::Actor* actor);
		~FaceGenMorphDetector();

		void update(RE::Actor* actor);

		uint32_t GetActiveMorphType() { return CurrentActiveFaceGenMorphType; };
		std::uint8_t GetActiveMorphValue() { return CurrentActiveFaceGenMorphValue; };

		enum class expression_type_simple : std::uint8_t {
			Natural = -1,

			Anger,
			Fear,
			Happy,
			Sad,
			Surprise,
			Puzzled,
			Disgust,

			Total
		};
		std::uint8_t GetEmotionSimpleType();

		void PlayerReset(RE::Actor* actor);
	private:
		bool PlayerRun = true;

		uint32_t CurrentActiveFaceGenMorphType = std::to_underlying(expression_type::mood_neutral);
		std::uint8_t CurrentActiveFaceGenMorphValue = 0;

		std::uint32_t IGetActiveMorphType();
		std::uint8_t IGetActiveMorphValue();


		bool GetFaceNodeNameList();
		std::unordered_map<std::string, uint8_t> FaceNodeNameList;
		bool updateEmotionMorphNodes();

		bool isValidMorphNodes = true;
		bool isNeedUpdatedMorph = false;
		bool SetAlphaEmotionMorph(RE::NiAVObject* obj, bool visible);
		bool SendFaceGenMorphEvent();
		clock_t beforeTime = 0;
		clock_t cooldown = 100; //0.1sec

		std::uint8_t ActiveLimit = 70;

		const RE::BSFixedString eventName = RE::BSFixedString("FaceGenMorphEvent");
		SKSE::ModCallbackEvent FaceGenMorphEvent = SKSE::ModCallbackEvent(eventName, "", 0.0f, nullptr);
	};
}