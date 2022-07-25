#include "Store.h"

namespace Mus {
	std::atomic<bool> IsRaceSexMenu = false;
	std::atomic<bool> IsMainMenu = false;
	bool IsValidTeraElinRace = false;

	std::atomic<bool> IsPlayerElin = false;

	std::atomic<bool> IsScanningScriptRace = true;

	namespace Nodes {
		const RE::BSFixedString& EarL11("HDT EarL1.001");
		const RE::BSFixedString& EarR11("HDT EarR1.001");
		const RE::BSFixedString& EarL12("HDT EarL1.002");
		const RE::BSFixedString& EarR12("HDT EarR1.002");
		const RE::BSFixedString& EarL13("HDT EarL1.003");
		const RE::BSFixedString& EarR13("HDT EarR1.003");
		
		const RE::BSFixedString& EarL21("HDT EarL2.001");
		const RE::BSFixedString& EarR21("HDT EarR2.001");
		const RE::BSFixedString& EarL22("HDT EarL2.002");
		const RE::BSFixedString& EarR22("HDT EarR2.002");
		const RE::BSFixedString& EarL23("HDT EarL2.003");
		const RE::BSFixedString& EarR23("HDT EarR2.003");
		
		const RE::BSFixedString& EarL31("HDT EarL3.001");
		const RE::BSFixedString& EarR31("HDT EarR3.001");
		const RE::BSFixedString& EarL32("HDT EarL3.002");
		const RE::BSFixedString& EarR32("HDT EarR3.002");


		const RE::BSFixedString& TailBone1("HDT TailBone01.1");
		const RE::BSFixedString& TailBone2("HDT TailBone01.2");
		const RE::BSFixedString& TailBone3("HDT TailBone01.3");

		const RE::BSFixedString& TailSpine("HDT Tail Spine1.001");
	}
}
