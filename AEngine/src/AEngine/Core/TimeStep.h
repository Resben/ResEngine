/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Timestep interval
**/
#pragma once
#include <chrono>

namespace AEngine
{
		/**
		 * \class TimeStep
		 * \brief Abstraction of a time interval
		**/
	class TimeStep
	{
		using interval = std::chrono::steady_clock::duration;
	public:
		TimeStep() = default;

			/**
			 * \brief Creates TimeStep from duration
			 * \param[in] step as a duration
			**/
		TimeStep(interval step);
			/**
			 * \brief Creates TimeStep from float in seconds
			 * \param[in] seconds interval time
			**/
		TimeStep(float seconds);

			/**
			 * \brief Gets timestep in nanoseconds
			 * \return float
			**/
		float Nanoseconds() const;
			/**
			 * \brief Gets timestep in microseconds
			 * \return float
			**/
		float Microseconds() const;
			/**
			 * \brief Gets timestep in milliseconds
			 * \return float
			**/
		float Milliseconds() const;
			/**
			 * \brief Gets timestep in seconds
			 * \return float
			**/
		float Seconds() const;
			/**
			 * \brief Implicit conversion to seconds as a float
			**/
		operator float() const;
			/**
			 * \brief Adds the given time step to this time step and returns the result as a new TimeStep.
			 * \param[in] rhs The time step to add.
			 * \return The sum of this time step and the given time step.
			 */
		TimeStep operator+(const TimeStep& rhs) const;
			/**
			 * \brief Assigns the value of the given time step to this time step.
			 * \param[in] rhs The time step to assign.
			 * \return A reference to this time step.
			 */
		TimeStep& operator=(const TimeStep& rhs);
			/**
			 * \brief Adds the given time step to this time step, and assigns the result to this time step.
			 * \param[in] rhs The time step to add.
			 * \return A reference to this time step.
			 */
		TimeStep& operator+=(const TimeStep& rhs);

	private:
		interval m_step;
	};
}
