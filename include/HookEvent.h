#pragma once

namespace Mus {
	using EventResult = RE::BSEventNotifyControl;

	class EventHandler final : 
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESLoadGameEvent>, 
		public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
		public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>/*,
		public RE::BSTEventSink<RE::InputEvent> */{

	public:
		static EventHandler& GetSingleton() {
			static EventHandler instance;
			return instance;
		}

		void Register(bool isLoaded);

	protected:
		EventResult ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		EventResult ProcessEvent(const RE::TESLoadGameEvent* evn, RE::BSTEventSource<RE::TESLoadGameEvent>*) override;
		EventResult ProcessEvent(const RE::TESObjectLoadedEvent* evn, RE::BSTEventSource<RE::TESObjectLoadedEvent>*) override;
		EventResult ProcessEvent(const RE::TESSwitchRaceCompleteEvent* evn, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>*) override;
		//EventResult ProcessEvent(const RE::InputEvent* evn, RE::BSTEventSource<RE::InputEvent>*) override;

	private:
		void MenuOpened(std::string name);
		void MenuClosed(std::string name);

		/*std::uint32_t EarLKey = 0x0;
		std::uint32_t EarRKey = 0x0;
		std::uint32_t TailKey = 0x0;*/
	};

	class InputEventHandler final : 
		public RE::BSTEventSink<RE::InputEvent*> {

	public:
		static InputEventHandler* GetSingleton() {
			static InputEventHandler* instance;
			return instance;
		}

	protected:
		virtual EventResult ProcessEvent(const RE::InputEvent** evn, RE::BSTEventSource<RE::InputEvent*>*) {
			if (isFirstRun)
			{
				EarLKey = Config::GetSingleton().GetSetting().GetAnimation().GetDirectControl().GetLEarHotkey();
				EarRKey = Config::GetSingleton().GetSetting().GetAnimation().GetDirectControl().GetREarHotkey();
				TailKey = Config::GetSingleton().GetSetting().GetAnimation().GetDirectControl().GetTailHotKey();
				isFirstRun = false;
			}

			if (!evn || !(*evn))
				return EventResult::kContinue;

			const RE::ButtonEvent* button = (*evn)->AsButtonEvent();
			if (!button)
				return EventResult::kContinue;

			if (button->IsPressed())
			{
				if (button->GetIDCode() == EarLKey)
				{
					PressEarLKey.store(true);
				}
				else if (button->GetIDCode() == EarRKey)
				{
					PressEarRKey.store(true);
				}
				else if (button->GetIDCode() == TailKey)
				{
					PressTailKey.store(true);
				}

				logger::trace("Press Button Event : {}", button->GetIDCode());
			}
			else
			{
				if (button->GetIDCode() == EarLKey)
				{
					PressEarLKey.store(false);
				}
				else if (button->GetIDCode() == EarRKey)
				{
					PressEarRKey.store(false);
				}
				else if (button->GetIDCode() == TailKey)
				{
					PressTailKey.store(false);
				}

				logger::trace("Unpress Button Event : {}", button->GetIDCode());
			}

			return EventResult::kContinue;
		};

	private:
		std::uint32_t EarLKey = 0x0;
		std::uint32_t EarRKey = 0x0;
		std::uint32_t TailKey = 0x0;

		bool isFirstRun = true;
	};
}