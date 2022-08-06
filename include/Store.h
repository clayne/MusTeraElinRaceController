#pragma once

namespace Mus {
	const RE::NiPoint3 emptyPoint = RE::NiPoint3(0, 0, 0);
	const RE::NiMatrix3 emptyRotate = RE::NiMatrix3(emptyPoint);

	const std::string ModPrefix = "TeraElinRace Controller : ";
	const std::string NodePrefix = "Mus ";

	extern std::atomic<bool> IsRaceSexMenu;
	extern std::atomic<bool> IsMainMenu;
	extern bool IsValidTeraElinRace;
	extern std::atomic<bool> IsPlayerElin;

	extern std::atomic<bool> PressEarLKey;
	extern std::atomic<bool> PressEarRKey;
	extern std::atomic<bool> PressTailKey;

	namespace Nodes {
		extern const RE::BSFixedString EarL11;
		extern const RE::BSFixedString EarR11;
		extern const RE::BSFixedString EarL12;
		extern const RE::BSFixedString EarR12;
		extern const RE::BSFixedString EarL13;
		extern const RE::BSFixedString EarR13;

		extern const RE::BSFixedString EarL21;
		extern const RE::BSFixedString EarR21;
		extern const RE::BSFixedString EarL22;
		extern const RE::BSFixedString EarR22;
		extern const RE::BSFixedString EarL23;
		extern const RE::BSFixedString EarR23;

		extern const RE::BSFixedString EarL31;
		extern const RE::BSFixedString EarR31;
		extern const RE::BSFixedString EarL32;
		extern const RE::BSFixedString EarR32;


		extern const RE::BSFixedString TailBone1;
		extern const RE::BSFixedString TailBone2;
		extern const RE::BSFixedString TailBone3;

		extern const RE::BSFixedString TailSpine;
	}
}
