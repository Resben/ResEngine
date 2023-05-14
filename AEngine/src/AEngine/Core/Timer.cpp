/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Basic start/stop timer implementation
**/
#include "Timer.h"

namespace AEngine
{
	void Timer::Start()
	{
		if (!IsRunning())
		{
			m_startTime = internal_clock::now();
		}
	}

	void Timer::Stop()
	{
		m_accumulator = ElapsedInternal();
		m_startTime = internal_clock::time_point{};
	}

	void Timer::Reset()
	{
		m_startTime = internal_clock::time_point{};
		m_accumulator = internal_clock::duration{};
	}

	TimeStep Timer::GetDelta()
	{
		// get timepoint of lap
		internal_clock::time_point now = internal_clock::now();

		// get elapsed time
		TimeStep lapTime = PeekDelta();

		// restart timer
		m_accumulator = internal_clock::duration{};
		m_startTime = now;

		// return the lap time
		return lapTime;
	}

	TimeStep Timer::PeekDelta()
	{
		return ElapsedInternal();
	}

	bool Timer::IsRunning() const
	{
		return m_startTime != internal_clock::time_point{};
	}

	Timer::internal_clock::duration Timer::ElapsedInternal()
	{
		internal_clock::duration elapsed = m_accumulator;
		if (IsRunning())
		{
			elapsed += internal_clock::now() - m_startTime;
		}

		return elapsed;
	}
}
