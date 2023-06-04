/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"

namespace AEngine
{
	class MessageAgent;

	class MessageService
	{
	public:
		static MessageAgent CreateAgent(Uint16 identifier);
	};
}
