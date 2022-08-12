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

		FaceGenMorphDetector(RE::Actor* actor, std::uint8_t EmotionEffectActiveThreshold);
		~FaceGenMorphDetector();

		void update(RE::Actor* actor);

		inline uint32_t GetActiveMorphType() { return CurrentActiveFaceGenMorphType; };
		inline std::uint8_t GetActiveMorphValue() { return CurrentActiveFaceGenMorphValue; };

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

		bool IsValidActor = true;
	private:
		bool PlayerRun = true;
		clock_t PlayerFirstCooldown = 10000; //10sec
		clock_t PlayerFirstTime = 0;

		uint32_t CurrentActiveFaceGenMorphType = std::to_underlying(expression_type::mood_neutral);
		std::uint8_t CurrentActiveFaceGenMorphValue = 0;

		std::uint32_t IGetActiveMorphType();
		std::uint8_t IGetActiveMorphValue();

		bool GetFaceNodeNameList();
		std::unordered_map<std::string, uint8_t> FaceNodeNameList;
		std::unordered_map<std::string, float> FaceNodeAlphaList;
		bool updateEmotionMorphNodes();

		bool isValidMorphNodes = true;
		bool isNeedUpdatedMorph = false;
		bool isNeedSendEvent = false;
		bool SetAlphaEmotionMorph(RE::NiAVObject* obj, float alpha);
		float GetAlphaEmotionMorph(RE::NiAVObject* obj);
		bool FixAlphaProperty(RE::NiAVObject* obj);
		bool SendFaceGenMorphEvent();
		clock_t beforeTime = 0;
		clock_t cooldown = 0; //msec

		std::uint8_t ActiveThreshold = 70;

		const RE::BSFixedString eventName = RE::BSFixedString("ElinFaceGenMorphEvent");
		SKSE::ModCallbackEvent FaceGenMorphEvent = SKSE::ModCallbackEvent(eventName, "", 0.0f, nullptr);

		bool isEnable = true;
		bool isEyes = true;
		bool isIcons = true;
		bool isTears = true;
		bool isOverlay = true;
		bool isPlayerOnly = false;
	};

	class RaceSexMenuTracker : public IEventListener<FrameEvent> {
	public:
		RaceSexMenuTracker();
		~RaceSexMenuTracker();

		[[nodiscard]] static RaceSexMenuTracker& GetSingleton() {
			static RaceSexMenuTracker instance;
			return instance;
		}

		bool InitTrackingList();
		bool TrackingRaceSexMenu();

		void DebugSliderPrint();

	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		RE::BSTArrayBase::size_type GetElinRaceindex();

		void AddSliderIndexOnLookupSliderMap(std::vector<RE::BSTArrayBase::size_type> sliderIndexs);
		void AddSliderNamesOnLookupSliderMap(std::vector<std::string> sliderNames);
		void GetTintMasks();

		void TrackingSliderMap();
		void TrackingTintMasks();

		clock_t cooldown = 1000; // 1sec
		clock_t currentTime = 0;
		bool FirstRun = true;

		bool SendSliderChangeEvent(std::string sliderName, RE::BSTArrayBase::size_type index, float numArg);
		bool SendTintChangeEvent(std::string_view typeName, std::uint32_t abgr);

		const RE::BSFixedString SliderEventName = RE::BSFixedString("ElinSliderChangeEvent");
		SKSE::ModCallbackEvent SliderChangeEvent = SKSE::ModCallbackEvent(SliderEventName, "", 0.0f, nullptr);

		const RE::BSFixedString TintEventName = RE::BSFixedString("ElinTintChangeEvent");
		SKSE::ModCallbackEvent TintChangeEvent = SKSE::ModCallbackEvent(TintEventName, "", 0.0f, nullptr);

		concurrency::concurrent_unordered_map<RE::BSTArrayBase::size_type, concurrency::concurrent_unordered_map<RE::SEX, float>> LookupSliderMap; //index, sex, value
		std::unordered_map<std::uint32_t, std::uint32_t> LookupTintMap; // type, abgr

		bool isScanDone = false;
		bool isFemale = true;

		RE::BSTArrayBase::size_type raceindex = 0;
	};
}

namespace RE
{
	class TintMask
	{
	public:
		TintMask()
		{
			alpha = 0.0;
			tintType = TintType::kMaskType_Frekles;
			texture = NULL;
		};
		~TintMask() { };

		enum TintType : std::uint32_t {
			kMaskType_Frekles = 0,
			kMaskType_Lips,
			kMaskType_Cheeks,
			kMaskType_Eyeliner,
			kMaskType_UpperEyeSocket,
			kMaskType_LowerEyeSocket,
			kMaskType_SkinTone,
			kMaskType_WarPaint,
			kMaskType_FrownLines,
			kMaskType_LowerCheeks,
			kMaskType_Nose,
			kMaskType_Chin,
			kMaskType_Neck,
			kMaskType_Forehead,
			kMaskType_Dirt,

			Total
		};
		RE::TESTexture* texture;

		union {
			struct Color {
				std::uint8_t red, green, blue, alpha; // The alpha isn't actually used here so its usually zero
			} color;
			std::uint32_t abgr;
		};

		float alpha;
		std::uint32_t tintType;

		std::uint32_t ToARGB();
	};
}