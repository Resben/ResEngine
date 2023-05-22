/**
 * @file
 * @author Christien Alden (34119981)
*/
#include "Timer.h"

namespace AEngine
{
	void Timer::Start()
	{
		if (!IsRunning())
		{
			m_startTime = Clock::now();
		}
	}

	void Timer::Stop()
	{
		m_accumulator = ElapsedInternal();
		m_startTime = Clock::time_point{};
	}

	void Timer::Reset()
	{
		m_startTime = Clock::time_point{};
		m_accumulator = Clock::duration{};
	}

	TimeStep Timer::GetDelta()
	{
		// get timepoint of lap
		Clock::time_point now = Clock::now();

		// get elapsed time
		TimeStep lapTime = PeekDelta();

		// restart timer
		m_accumulator = Clock::duration{};
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
		return m_startTime != Clock::time_point{};
	}

	Timer::Clock::duration Timer::ElapsedInternal()
	{
		Clock::duration elapsed = m_accumulator;
		if (IsRunning())
		{
			elapsed += Clock::now() - m_startTime;
		}

		return elapsed;
	}
}
