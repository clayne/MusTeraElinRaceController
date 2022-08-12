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

		void Register();

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

	class InputEventHandler : 
		public RE::BSTEventSink<RE::InputEvent*> {
	public:
		static InputEventHandler* GetSingleton() {
			static InputEventHandler* instance;
			return instance;
		}

		void Register();

	protected:
		EventResult ProcessEvent(const RE::InputEvent*& evn, RE::BSTEventSource<RE::InputEvent*>*);

	private:
		std::uint32_t EarLKey = 0x0;
		std::uint32_t EarRKey = 0x0;
		std::uint32_t TailKey = 0x0;

		bool isFirstRun = true;
	};
}