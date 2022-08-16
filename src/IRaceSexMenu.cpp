#include "RE/R/RaceSexMenu.h"

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