#include "EventHandler.h"

namespace AEngine
{
	void EventHandler::PushEvent(UniquePtr<Event> event)
	{
		// the correct event handlers are found by type and then called by priority
		EventCallbackListByPriority& handlers = m_eventHandlers[event->GetType()];
		for (auto& [layer, callbacks] : handlers)
		{
			for (auto& callback : callbacks)
			{
				if (callback(*event))
				{
					return;
				}
			}
		}
	}
}