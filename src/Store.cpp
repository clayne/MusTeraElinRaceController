#include "Store.h"

namespace Mus {
	std::atomic<bool> IsRaceSexMenu = false;
	std::atomic<bool> IsMainMenu = false;
	bool IsValidTeraElinRace = false;

	std::atomic<bool> IsPlayerElin = false;

	std::atomic<bool> PressEarLKey = false;
	std::atomic<bool> PressEarRKey = false;
	std::atomic<bool> PressTailKey = false;

	namespace Nodes {
		const RE::BSFixedString EarL11 = RE::BSFixedString("HDT EarL1.001");
		const RE::BSFixedString EarR11 = RE::BSFixedString("HDT EarR1.001");
		const RE::BSFixedString EarL12 = RE::BSFixedString("HDT EarL1.002");
		const RE::BSFixedString EarR12 = RE::BSFixedString("HDT EarR1.002");
		const RE::BSFixedString EarL13 = RE::BSFixedString("HDT EarL1.003");
		const RE::BSFixedString EarR13 = RE::BSFixedString("HDT EarR1.003");
		
		const RE::BSFixedString EarL21 = RE::BSFixedString("HDT EarL2.001");
		const RE::BSFixedString EarR21 = RE::BSFixedString("HDT EarR2.001");
		const RE::BSFixedString EarL22 = RE::BSFixedString("HDT EarL2.002");
		const RE::BSFixedString EarR22 = RE::BSFixedString("HDT EarR2.002");
		const RE::BSFixedString EarL23 = RE::BSFixedString("HDT EarL2.003");
		const RE::BSFixedString EarR23 = RE::BSFixedString("HDT EarR2.003");
		
		const RE::BSFixedString EarL31 = RE::BSFixedString("HDT EarL3.001");
		const RE::BSFixedString EarR31 = RE::BSFixedString("HDT EarR3.001");
		const RE::BSFixedString EarL32 = RE::BSFixedString("HDT EarL3.002");
		const RE::BSFixedString EarR32 = RE::BSFixedString("HDT EarR3.002");


		const RE::BSFixedString TailBone1 = RE::BSFixedString("HDT TailBone01.1");
		const RE::BSFixedString TailBone2 = RE::BSFixedString("HDT TailBone01.2");
		const RE::BSFixedString TailBone3 = RE::BSFixedString("HDT TailBone01.3");

		const RE::BSFixedString TailSpine = RE::BSFixedString("HDT Tail Spine1.001");
	}
}
