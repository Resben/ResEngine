#include "MessageService.h"
#include "MessageServiceImpl.h"

namespace AEngine
{
	MessageAgent MessageService::CreateAgent(Uint16 identifier)
	{
		return MessageServiceImpl::Instance().CreateAgent(identifier);
	}

	void MessageService::DispatchMessages()
	{
		MessageServiceImpl::Instance().DispatchMessages();
	}
}
