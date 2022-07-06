#pragma once

namespace Mus {
	extern std::string ModPrefix;
	extern std::string UsePrefix;

	extern std::atomic<bool> IsRaceSexMenu;
	extern std::atomic<bool> IsMainMenu;
	extern bool IsValidTeraElinRace;
	extern std::atomic<bool> IsPlayerElin;

	namespace Nodes {
		extern RE::BSFixedString EarL11;
		extern RE::BSFixedString EarR11;
		extern RE::BSFixedString EarL12;
		extern RE::BSFixedString EarR12;
		extern RE::BSFixedString EarL13;
		extern RE::BSFixedString EarR13;

		extern RE::BSFixedString EarL21;
		extern RE::BSFixedString EarR21;
		extern RE::BSFixedString EarL22;
		extern RE::BSFixedString EarR22;
		extern RE::BSFixedString EarL23;
		extern RE::BSFixedString EarR23;

		extern RE::BSFixedString EarL31;
		extern RE::BSFixedString EarR31;
		extern RE::BSFixedString EarL32;
		extern RE::BSFixedString EarR32;


		extern RE::BSFixedString TailBone1;
		extern RE::BSFixedString TailBone2;
		extern RE::BSFixedString TailBone3;

		extern RE::BSFixedString TailSpine;
	}
}
