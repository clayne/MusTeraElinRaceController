#pragma once

#include "detours.h"

namespace Mus {
#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define CALL_MEMBER_FN(obj, fn) ((*(obj)).*(*((obj)->_##fn##_GetPtr())))

#define DEFINE_MEMBER_FN_LONG_HOOK(className, functionName, retnType, address, ...) \
    typedef retnType (className::*_##functionName##_type)(__VA_ARGS__);             \
    static inline uintptr_t* _##functionName##_GetPtrAddr(void) {                   \
        static uintptr_t _address = address;	\
        return &_address;                                                           \
    }                                                                               \
                                                                                    \
    static inline _##functionName##_type* _##functionName##_GetPtr(void) {          \
        return (_##functionName##_type*)_##functionName##_GetPtrAddr();             \
    }

#define DEFINE_MEMBER_FN_HOOK(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG_HOOK(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)


    template <typename T>
    uintptr_t GetFnAddr(T src) {
        union {
            uintptr_t u;
            T t;
        } data;

        data.t = src;

        return data.u;
    }

	struct FrameEvent
	{
		bool gamePaused;
	};


	template <class Event = void>
	class IEventListener
	{
	public:
		virtual void onEvent(const Event&) = 0;
	};

	template <>
	class IEventListener<void>
	{
	public:
		virtual void onEvent() = 0;
	};

	template <class Event = void>
	class IEventDispatcher
	{
	public:

		virtual ~IEventDispatcher()
		{
		}

		virtual void addListener(IEventListener<Event>*) = 0;
		virtual void removeListener(IEventListener<Event>*) = 0;
		virtual void dispatch(const Event&) = 0;
	};

	template <>
	class IEventDispatcher<void>
	{
	public:

		virtual ~IEventDispatcher()
		{
		}

		virtual void addListener(IEventListener<void>*) = 0;
		virtual void removeListener(IEventListener<void>*) = 0;
		virtual void dispatch() = 0;
	};

	template <class Event = void>
	class EventDispatcherImpl : public IEventDispatcher<Event>
	{
	public:
		EventDispatcherImpl()
		{
		}

		~EventDispatcherImpl()
		{
		}

		void addListener(IEventListener<Event>*) override;
		void removeListener(IEventListener<Event>*) override;
		void dispatch(const Event&) override;

	protected:
		std::unordered_set<IEventListener<Event>*> m_listeners;
		std::vector<IEventListener<Event>*> m_caches;
		bool m_cacheDirt = false;
	};

	template <class Event>
	void EventDispatcherImpl<Event>::addListener(IEventListener<Event>* listener)
	{
		m_listeners.insert(listener);
		m_cacheDirt = true;
	}

	template <class Event>
	void EventDispatcherImpl<Event>::removeListener(IEventListener<Event>* listener)
	{
		m_listeners.erase(listener);
		m_cacheDirt = true;
	}

	template <class Event>
	void EventDispatcherImpl<Event>::dispatch(const Event& event)
	{
		if (m_cacheDirt)
		{
			m_caches.clear();
			for (auto& i : m_listeners)
				m_caches.emplace_back(i);
			m_cacheDirt = false;
		}

		for (auto i : m_caches)
			i->onEvent(event);
	}

	template <>
	class EventDispatcherImpl<void> : public IEventDispatcher<void>
	{
	public:
		EventDispatcherImpl()
		{
		}

		~EventDispatcherImpl()
		{
		}

		void addListener(IEventListener<void>*) override;
		void removeListener(IEventListener<void>*) override;
		void dispatch() override;

	protected:
		std::unordered_set<IEventListener<void>*> m_listeners;
		std::vector<IEventListener<void>*> m_caches;
		bool m_cacheDirt = false;
	};

	inline void EventDispatcherImpl<void>::dispatch()
	{
		if (m_cacheDirt)
		{
			m_caches.clear();
			for (auto& i : m_listeners)
				m_caches.emplace_back(i);
			m_cacheDirt = false;
		}

		for (auto i : m_caches)
			i->onEvent();

	}

	extern EventDispatcherImpl<FrameEvent>  g_frameEventDispatcher;

	void hook();
	void unhook();
}
