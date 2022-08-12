#include "PlayerGenderDetector.h"

namespace Mus {
	PlayerGenderDetector::PlayerGenderDetector()
	{

	}

	PlayerGenderDetector::~PlayerGenderDetector()
	{

	}

	void PlayerGenderDetector::onEvent(const FrameEvent& e)
	{
		if (!IsRaceSexMenu.load())
			return;

		if (!FirstRun)
			PlayerFemale = IsPlayerFemale();

		bool CurrentSex = IsPlayerFemale();

		if (PlayerFemale != CurrentSex)
		{
			RE::BSTEventSource<SKSE::ModCallbackEvent>* modEvent = SKSE::GetModCallbackEventSource();

			if (modEvent)
			{
				genderEvent.sender = RE::PlayerCharacter::GetSingleton();

				modEvent->SendEvent(&genderEvent);
			}
		}

		PlayerFemale = CurrentSex;
	}
}

