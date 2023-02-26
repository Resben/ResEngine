/**
 * @file
 * @author Christien Alden (34119981)
**/
#include "AEngine/Core/IntervalTimer.h"

namespace AEngine
{
	void IntervalTimer::Resume()
	{
		if (!IsRunning())
		{
			m_startTime = clock::now();
		}
	}

	void IntervalTimer::Pause()
	{
		if (IsRunning())
		{
			m_accumulator += (clock::now() - m_startTime);
			m_startTime = clock::time_point{};
		}
	}

	TimeStep IntervalTimer::Update()
	{
		clock::duration dt = m_accumulator;
		if (IsRunning())
		{
			clock::time_point now = clock::now();
			dt += now - m_startTime;
			m_startTime = now;
		}

		m_accumulator = clock::duration{};
		return dt;
	}

	void IntervalTimer::Reset()
	{
		m_startTime = clock::now();
		m_accumulator = clock::duration{};
	}

	bool IntervalTimer::IsRunning() const
	{
		return m_startTime != clock::time_point{};
	}
}
