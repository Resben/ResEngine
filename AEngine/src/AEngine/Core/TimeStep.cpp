/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Timestep interval implementation
**/
#include "TimeStep.h"

namespace AEngine
{
	static constexpr float ratio = 1000.0f;

	TimeStep::TimeStep(interval step)
		: m_step(step)
	{

	}

	TimeStep::TimeStep(float seconds)
		: m_step()
	{
		// from StackOverflow answer -> https://stackoverflow.com/a/69199519
		using namespace std::chrono;
		using fsec = duration<float>;
		m_step = round<nanoseconds>(fsec{seconds});
	}

	float TimeStep::Nanoseconds() const
	{
		return static_cast<float>(m_step.count());
	}

	float TimeStep::Microseconds() const
	{
		return Nanoseconds() / ratio;
	}

	float TimeStep::Milliseconds() const
	{
		return Microseconds() / ratio;
	}

	float TimeStep::Seconds() const
	{
		return Milliseconds() / ratio;
	}

	TimeStep::operator float() const
	{
		return Seconds();
	}

	TimeStep TimeStep::operator+(const TimeStep &rhs) const
	{
		return TimeStep{m_step + rhs.m_step};
	}

	TimeStep& TimeStep::operator=(const TimeStep &rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		this->m_step = rhs.m_step;
		return *this;
	}

	TimeStep& TimeStep::operator+=(const TimeStep &rhs)
	{
		m_step = m_step + rhs.m_step;
		return *this;
	}
}
