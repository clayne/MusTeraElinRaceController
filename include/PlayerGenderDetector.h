#pragma once


namespace Mus {
	class PlayerGenderDetector
		: public IEventListener<FrameEvent>
	{
	public:
		PlayerGenderDetector();
		~PlayerGenderDetector();

		[[nodiscard]] static PlayerGenderDetector& GetSingleton() {
			static PlayerGenderDetector instance;
			return instance;
		}

	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		bool PlayerFemale = false; //true is female / false is male
		bool FirstRun = false;

		inline bool IsPlayerFemale() //true is female / false is male
		{
			RE::Actor* Player = RE::PlayerCharacter::GetSingleton();

			if (!Player)
				return PlayerFemale;

			RE::TESNPC* TESPlayer = Player->GetActorBase();

			return (TESPlayer->GetSex() == 0 ? false : true);
		};

		RE::BSFixedString eventName = "PlayerGenderChangeEvent";
		SKSE::ModCallbackEvent genderEvent = SKSE::ModCallbackEvent(eventName, "", 0.0f, nullptr);
	};
}
