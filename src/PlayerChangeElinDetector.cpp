#include "PlayerChangeElinDetector.h"


namespace Mus {
	PlayerChangeElinDetector::PlayerChangeElinDetector()
	{

	}

	PlayerChangeElinDetector::~PlayerChangeElinDetector()
	{

	}

	void PlayerChangeElinDetector::onEvent(const FrameEvent& e)
	{
		if (!IsRaceSexMenu.load())
		{
			if (IsRun)
				IsRun = false;

			return;
		}

		if (!IsRun && !IsPlayerElin.load() && CheckPlayerIsElin())
		{
			RaceCompatibility::GetSingleton().LoadRaceCompatibility();
			RaceCompatibility::GetSingleton().RemoveHeadPartElinRacesForm();

			IsRun = true;
		}
	}

	bool PlayerChangeElinDetector::CheckPlayerIsElin()
	{
		IsPlayerElin.store(RaceCompatibility::GetSingleton().isPlayerRaceTeraElinHumanoidVampireLord());

		return IsPlayerElin.load();
	}
}
