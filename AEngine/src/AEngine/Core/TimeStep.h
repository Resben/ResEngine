/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include <chrono>

namespace AEngine
{
		/**
		 * \class TimeStep
		 * \brief Abstraction of a time interval
		*/
	class TimeStep
	{
	private:
		using Interval = std::chrono::steady_clock::duration;

	public:
		TimeStep() = default;

			/**
			 * \brief Creates TimeStep from duration
			 * \param[in] step as a duration
			*/
		TimeStep(Interval step);
			/**
			 * \brief Creates TimeStep from float in seconds
			 * \param[in] seconds interval time
			*/
		TimeStep(float seconds);

			/**
			 * \brief Gets timestep in nanoseconds
			 * \param[in] scale to multiply by (default 1.0f)
			 * \return float
			*/
		float Nanoseconds(float scale = 1.0f) const;
			/**
			 * \brief Gets timestep in microseconds
			 * \param[in] scale to multiply by (default 1.0f)
			 * \return float
			*/
		float Microseconds(float scale = 1.0f) const;
			/**
			 * \brief Gets timestep in milliseconds
			 * \param[in] scale to multiply by (default 1.0f)
			 * \return float
			*/
		float Milliseconds(float scale = 1.0f) const;
			/**
			 * \brief Gets timestep in seconds
			 * \param[in] scale to multiply by (default 1.0f)
			 * \return float
			*/
		float Seconds(float scale = 1.0f) const;
			/**
			 * \brief Implicit conversion to seconds as a float
			*/
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
			/**
			 * \brief Subtracts the given time step to this time step, and assigns the result to this time step.
			 * \param[in] rhs The time step to subtract.
			 * \return A reference to this time step.
			 */
		TimeStep& operator-=(const TimeStep& rhs);

	private:
		Interval m_step;
	};
}
