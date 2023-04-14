/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Timestep interval
**/
#pragma once
#include <chrono>

namespace AEngine
{
		/**
		 * @class TimeStep
		 * @brief Abstraction of a time interval
		**/
	class TimeStep
	{
		using interval = std::chrono::steady_clock::duration;
	public:
		TimeStep() = default;

			/**
			 * @brief Creates TimeStep from duration
			 * @param[in] step as a duration
			**/
		TimeStep(interval step);
			/**
			 * @brief Creates TimeStep from float in seconds
			 * @param[in] seconds interval time
			**/
		TimeStep(float seconds);

			/**
			 * @brief Gets timestep in nanoseconds
			 * @return float
			**/
		float Nanoseconds() const;
			/**
			 * @brief Gets timestep in microseconds
			 * @return float
			**/
		float Microseconds() const;
			/**
			 * @brief Gets timestep in milliseconds
			 * @return float
			**/
		float Milliseconds() const;
			/**
			 * @brief Gets timestep in seconds
			 * @return float
			**/
		float Seconds() const;
			/**
			 * @brief Returns timestep in seconds implicitly
			**/
		operator float() const;

	private:
		interval m_step;
	};
}
