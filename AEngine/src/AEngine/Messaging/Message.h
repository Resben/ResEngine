/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once

namespace AEngine
{
	struct Message
	{
		Uint16 sender;
		Uint16 receiver;
		Uint8 messageType;
		void* data;
	};
}
