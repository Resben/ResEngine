#pragma once
#include "Types.h"

namespace AEngine
{
	class Identifier
	{
	public:
		static Uint16 Generate();

	private:
		static Uint16 s_next;
	};
}
