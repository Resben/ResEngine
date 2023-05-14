#include "Logger.h"
#include "Identifier.h"

namespace AEngine
{
	uint16_t Identifier::s_next = 0;

	uint16_t Identifier::Generate()
	{
		if (s_next == UINT16_MAX)
		{
			AE_LOG_FATAL("Identifier::Generate::Error -> Limit reached");
		}

		return s_next++;
	}
}
