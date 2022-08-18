#include "RE/R/RaceSexMenu.h"

// RaceSexMenu.h
//
//namespace RE
//{
//	class RaceMenuSlider
//	{
//	public:
//		RaceMenuSlider() {};
//		RaceMenuSlider(std::uint32_t filterFlag, const char* sliderName, const char* callbackName, std::uint32_t sliderId, std::uint32_t index, std::uint32_t type, std::uint8_t unk8, float min, float max, float value, float interval, std::uint32_t unk13);
//
//		enum SliderType : std::uint32_t {
//			kTypeHeadPart = 0,
//			kTypeUnk1,
//			kTypeDoubleMorph,
//			kTypePreset,
//			kTypeTintingMask,
//			kTypeHairColorPreset,
//			kTypeUnk6,
//			kTypeUnused7,
//			kTypeUnk8,
//			kTypeUnk9,
//			kTypeUnk10
//		};
//
//		float	min;	// 00
//		float	max;	// 04
//		float	value;	// 08
//		float	interval;	// 0C
//		std::uint32_t	filterFlag;	// 10
//		std::uint32_t	type;	// 14
//		const char* name;	// 18
//		char	callback[MAX_PATH];	// 20
//		std::uint32_t	index;	// 124
//		std::uint32_t	id;	// 128
//		std::uint32_t	unk12C;	// 12C
//		std::uint32_t	unk130;	// 130 - 0x7F7FFFFF
//		std::uint8_t	unk134;	// 134
//		std::uint8_t	pad135[3]; // 135
//	};
//	static_assert(sizeof(RaceMenuSlider) == 0x138);
//
//	// menuDepth = 3
//	// flags = kPausesGame | kUsesMenuContext | kRequiresUpdate | kTopmostRenderedMenu | kUpdateUsesCursor
//	// context = kItemMenu
//	class RaceSexMenu :
//#ifndef SKYRIM_CROSS_VR
//		public IMenu,            // 00
//		public MenuEventHandler  // 30, 40
//#else
//		public IMenu  // 00
//#endif
//	{
//	public:
//		inline static constexpr auto      RTTI = RTTI_RaceSexMenu;
//		constexpr static std::string_view MENU_NAME = "RaceSex Menu";
//
//		enum HeadPartType : std::uint32_t {
//			kHeadPartsHairLine = 0,
//			kHeadPartsHead,
//			kHeadPartsEyes,
//			kHeadPartsHair,
//			kHeadPartsBeard,
//			kHeadPartsScars,
//			kHeadPartsBrows,
//			kNumHeadPartLists
//		};
//
//		struct RaceComponent
//		{
//			TESRace* race;			/*00*/
//			BSTArray<RaceMenuSlider>	sliders;	/*08*/
//			std::uint32_t				unk20;			/*20*/
//			std::uint32_t				pad24;			/*24*/
//		};
//
//		struct RUNTIME_DATA
//		{
//#define RUNTIME_DATA_CONTENT                                  \
//	BSTArray<BGSHeadPart*>               unk040[kNumHeadPartLists]; /* 040 */ \
//	RaceSexCamera                        camera;    /* 0A8 */ \
//	BSTArray<RaceComponent>              sliderData[RE::SEX::kTotal]; /* 100 */ \
//	BSTArray<void*>                      unk170;    /* 130 */ \
//	std::uint64_t                        unk188;    /* 148 */ \
//	std::uint32_t                        unk190;    /* 150 */ \
//	std::uint32_t                        unk194;    /* 154 */ \
//	stl::enumeration<SEX, std::uint32_t> sex;       /* 158 */ \
//	std::uint16_t                        unk19C;    /* 15C */ \
//	std::uint8_t                         unk19E;    /* 15E */ \
//	std::uint8_t                         pad19F;    /* 15F */ \
//	std::uint8_t                         unk1A0;    /* 160 */ \
//	std::uint8_t                         unk1A1;    /* 161 */ \
//	std::uint16_t                        unk1A2;    /* 162 */ \
//	std::uint32_t                        unk1A4;    /* 164 */
//
//			RUNTIME_DATA_CONTENT
//		};
//		static_assert(sizeof(RUNTIME_DATA) == 0x168);
//
//		~RaceSexMenu() override;  // 00
//
//		// override (IMenu)
//		void               Accept(CallbackProcessor* a_processor) override;                       // 01
//		void               PostCreate() override;                                                 // 02
//		UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;                         // 04
//		void               AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;  // 05
//
//		// override (MenuEventHandler)
//#ifndef SKYRIM_CROSS_VR
//		bool CanProcess(InputEvent* a_event) override;              // 01
//		bool ProcessThumbstick(ThumbstickEvent* a_event) override;  // 03
//		bool ProcessMouseMove(MouseMoveEvent* a_event) override;    // 04
//		bool ProcessButton(ButtonEvent* a_event) override;          // 05
//#endif
//
//		[[nodiscard]] MenuEventHandler* AsMenuEventHandler() noexcept
//		{
//			return &REL::RelocateMember<MenuEventHandler>(this, 0x30, 0x40);
//		}
//
//		[[nodiscard]] const MenuEventHandler* AsMenuEventHandler() const noexcept
//		{
//			return const_cast<RaceSexMenu*>(this)->AsMenuEventHandler();
//		}
//
//		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
//		{
//			return REL::RelocateMember<RUNTIME_DATA>(this, 0x40, 0x50);
//		}
//
//		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
//		{
//			return REL::RelocateMember<RUNTIME_DATA>(this, 0x40, 0x50);
//		}
//
//		// members
//#ifndef SKYRIM_CROSS_VR
//		RUNTIME_DATA_CONTENT  // 40, 50
//#endif
//	};
//#ifndef ENABLE_SKYRIM_VR
//	static_assert(sizeof(RaceSexMenu) == 0x1A8);
//#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
//	static_assert(sizeof(RaceSexMenu) == 0x1B8);
//#endif
//}
//#undef RUNTIME_DATA_CONTENT


namespace RE {
	RaceMenuSlider::RaceMenuSlider(std::uint32_t _filterFlag, const char* _sliderName, const char* _callbackName, std::uint32_t _sliderId, std::uint32_t _index, std::uint32_t _type, std::uint8_t _unk8, float _min, float _max, float _value, float _interval, std::uint32_t _unk13)
	{
		filterFlag = _filterFlag;
		name = _sliderName;
		strcpy_s(callback, MAX_PATH, _callbackName);
		id = _sliderId;
		index = _index;
		type = _type;
		min = _min;
		max = _max;
		value = _value;
		interval = _interval;
		unk12C = _unk13;
		unk130 = 0x7F7FFFFF;
		unk134 = _unk8;
		pad135[0] = 0;
		pad135[1] = 0;
		pad135[2] = 0;
	}
}