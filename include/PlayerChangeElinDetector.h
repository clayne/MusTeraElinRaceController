#pragma once


namespace Mus {
	class PlayerChangeElinDetector
		: public IEventListener<FrameEvent>
	{
	public:
		PlayerChangeElinDetector();
		~PlayerChangeElinDetector();

		[[nodiscard]] static PlayerChangeElinDetector& GetSingleton() {
			static PlayerChangeElinDetector instance;
			return instance;
		}

	protected:
		void onEvent(const FrameEvent& e) override;

	private:
		bool CheckPlayerIsElin();

		bool IsRun = false;
	};
}