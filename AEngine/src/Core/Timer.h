/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Basic start/stop timer
**/
#pragma once
#include <chrono>
#include "TimeStep.h"

namespace AEngine
{
		/**
		 * @class Timer
		 * @brief Simple start/stop timer that can be used to track elapsed time
		**/
	class Timer
	{
	public:
		using clock = std::chrono::steady_clock;
			/**
			 * @brief Resume timer, akin to starting a new 'lap'
			 * @return void
			**/
		void Resume();
			/**
			 * @brief Pause timer, saving any accumulated time
			 * @return void
			**/
		void Pause();
			/**
			 * @brief Stops timer and resets state to zero
			 * @return void
			**/
		void Reset();
			/**
			 * @brief Gets the elapsed running time
			 * @return TimeStep
			**/
		TimeStep GetElapsed() const;
			/**
			 * @brief Returns timer running state
			 * @retval true if running
			 * @retval false if not running
			**/
		bool IsRunning() const;

	private:
			/// Start time of current 'lap'
		clock::time_point m_startTime {};
			/// Accumulated time from previous 'lap'
		clock::duration m_accumulator {};
	};
}
