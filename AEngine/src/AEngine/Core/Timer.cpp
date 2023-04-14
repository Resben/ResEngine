/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Basic start/stop timer implementation
**/
#include "Timer.h"

namespace AEngine
{
	using internal_clock = std::chrono::steady_clock;

	Timer::Timer()
	{
		reset();
	}

	void Timer::start()
	{
		if (!isRunning())
		{
			m_startTime = internal_clock::now();
		}
	}

	void Timer::stop()
	{
		m_accumulator = elapsedInternal();
		m_startTime = internal_clock::time_point{};
	}

	void Timer::reset()
	{
		m_startTime = internal_clock::time_point{};
		m_accumulator = internal_clock::duration{};
	}

	TimeStep Timer::update()
	{
		// get timepoint of lap
		internal_clock::time_point now = internal_clock::now();

		// get elapsed time
		TimeStep lapTime = elapsed();

		// restart timer
		m_accumulator = internal_clock::duration{};
		m_startTime = now;

		// return the lap time
		return lapTime;
	}

	TimeStep Timer::elapsed()
	{
		return elapsedInternal();
	}

	bool Timer::isRunning() const
	{
		return m_startTime != internal_clock::time_point{};
	}


	internal_clock::duration Timer::elapsedInternal()
	{
		internal_clock::duration elapsed = m_accumulator;
		if (isRunning())
		{
			elapsed += internal_clock::now() - m_startTime;
		}

		return elapsed;
	}
}
