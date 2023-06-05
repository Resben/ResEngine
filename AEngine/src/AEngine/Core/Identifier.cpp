/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Identifier.h"
#include "Logger.h"

namespace AEngine
{
	Uint16 Identifier::s_next = 0;

	Uint16 Identifier::Generate()
	{
		if (s_next == UINT16_MAX)
		{
			AE_LOG_FATAL("Identifier::Generate::Fatal -> Identifier limit reached '{}'", UINT16_MAX);
		}

		return s_next++;
	}
}
