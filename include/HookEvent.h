#pragma once

namespace Mus {
	using EventResult = RE::BSEventNotifyControl;

	class EventHandler final : 
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESLoadGameEvent> {
	public:
		static EventHandler& GetSingleton() {
			static EventHandler instance;
			return instance;
		}

		void Register();

	protected:
		EventResult ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		EventResult ProcessEvent(const RE::TESLoadGameEvent* evn, RE::BSTEventSource<RE::TESLoadGameEvent>*) override;

	private:
		void MenuOpened(std::string name);
		void MenuClosed(std::string name);
	};
}