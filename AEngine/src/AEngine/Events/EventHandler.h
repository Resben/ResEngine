#pragma once
#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "AEngine/Core/Types.h"
#include <functional>
#include <map>
#include <vector>
#define AE_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace AEngine
{
	class EventHandler
	{
	public:
			/**
			 * \copydoc Window::RegisterEventHandler
			*/
		template <typename T>
		void RegisterHandler(int priority, const std::function<bool(T&)>& handler);
			/**
			 * \brief Push an event to the handlers
			 * \param event Event to push
			*/
		void PushEvent(UniquePtr<Event> event);

	private:
		using EventCallback = std::function<bool(Event&)>;
		using EventCallbackList = std::vector<EventCallback>;
		using EventCallbackListByPriority = std::map<int, EventCallbackList>;
		using EventHandlers = std::map<EventType, EventCallbackListByPriority>;

		EventHandlers m_eventHandlers;
	};

	template <typename T>
	void EventHandler::RegisterHandler(int layer, const std::function<bool(T&)>& handler)
	{
		m_eventHandlers[T::GetStaticType()][layer].push_back([=](Event& event) -> bool {
			return handler(dynamic_cast<T&>(event));
		});
	}
}
