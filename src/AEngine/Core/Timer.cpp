/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Basic start/stop timer implementation
**/
#include "AEngine/Core/Timer.h"

namespace AEngine
{
	using clock = std::chrono::steady_clock;

	void Timer::Resume()
	{
		if (!IsRunning())
		{
			m_startTime = clock::now();
		}
	}

	void Timer::Pause()
	{
		if (IsRunning())
		{
			m_accumulator += (clock::now() - m_startTime);
			m_startTime = clock::time_point{};
		}
	}

	void Timer::Reset()
	{
		m_startTime = clock::time_point{};
		m_accumulator = clock::duration{};
	}

	TimeStep Timer::GetElapsed() const
	{
		clock::duration elapsed = m_accumulator;
		if (IsRunning())
		{
			elapsed += clock::now() - m_startTime;
		}
			
		return elapsed;
	}

	bool Timer::IsRunning() const
	{
		return m_startTime != clock::time_point{};
	}
}
