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
		using internal_clock = std::chrono::steady_clock;

		Timer();
		void start();
		void stop();
		void reset();
		TimeStep update();
		TimeStep elapsed();
		bool isRunning() const;

	private:
		/// Start time of current 'update'
		internal_clock::time_point m_startTime;
		/// Accumulated time from previous 'update'
		internal_clock::duration m_accumulator;

		internal_clock::duration elapsedInternal();
	};
}
