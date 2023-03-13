/**
 * @file
 * @author Christien Alden (34119981)
 * @todo Re-work method names
**/
#pragma once
#include <chrono>
#include "TimeStep.h"

namespace AEngine
{
		/**
		 * @class IntervalTimer
		 * @author Christien Alden (34119981)
		 * @brief Provides a standard engine wide delta time object
		**/
	class IntervalTimer
	{
	public:
		using clock = std::chrono::steady_clock;
			/**
			 * @brief Sets IntervalTimer to a running state
			 * @retval void
			**/
		void Resume();
			/**
			 * @brief Sets IntervalTimer to a paused state
			 * @retval void
			 *
			 * The time duration between this call and the last call to Update
			 * will be added to the accumulator.
			**/
		void Pause();
			/**
			 * @brief Returns the TimeStep since last Update
			 * @return
			 * If running -> Interval since last Update plus any accumulated \n
			 * If paused -> Any accumulated interval and sets accumulated to zero
			**/
		TimeStep Update();

			/**
			 * @brief Discards accumulated time and restarts timer
			 * @retval
			**/
		void Reset();

			/**
			 * @brief Returns IntervalTimer running state
			 * @retval true if running
			 * @retval false if **not** running
			**/
		bool IsRunning() const;

	private:
		clock::time_point m_startTime{};
		clock::duration m_accumulator{};
	};
}
