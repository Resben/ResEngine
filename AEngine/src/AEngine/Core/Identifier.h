#pragma once
#include <cstdint>

namespace AEngine
{
	class Identifier
	{
	public:
		static uint16_t Generate();

	private:
		static uint16_t s_next;
	};
}
