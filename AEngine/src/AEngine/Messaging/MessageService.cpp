#include "MessageService.h"
#include "MessageAgent.h"
#include "MessageServiceImpl.h"

namespace AEngine
{
	MessageAgent MessageService::CreateAgent(Uint16 identifier)
	{
		return MessageServiceImpl::Instance().CreateAgent(identifier);
	}
}
