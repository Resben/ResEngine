/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include <chrono>
#include "TimeStep.h"

namespace AEngine
{
		/**
		 * \class Timer
		 * \brief Simple start/stop timer that can be used to track elapsed time
		*/
	class Timer
	{
	public:
			/**
			 * \brief Constructor
			*/
		Timer() = default;
			/**
			 * \brief Destructor
			*/
		~Timer() = default;
			/**
			 * \brief Starts the timer.
			*/
		void Start();
			/**
			 * \brief Stops the timer.
			*/
		void Stop();
			/**
			 * \brief Resets the timer.
			 * \note This will stop the timer.
			*/
		void Reset();
			/**
			 * \brief Gets the elapsed time since the last time GetDelta was called
			 * \return The elapsed time of this cycle
			 * \note This will reset the accumulator and start a new cycle.
			*/
		TimeStep GetDelta();
			/**
			 * \brief Gets the elapsed time since the last time GetDelta was called
			 * \return The elapsed time of this cycle
			 * \note This will not reset the accumulator or start a new cycle.
			*/
		TimeStep PeekDelta();
			/**
			 * \brief Checks if the timer is running.
			 * \retval true The timer is running.
			 * \retval false The timer is not running.
			*/
		bool IsRunning() const;

	private:
		using Clock = std::chrono::steady_clock;

			/**
			 * \brief Start time of current 'update'
			*/
		Clock::time_point m_startTime;
			/**
			 * \brief Accumulated time from previous 'update'
			*/
		Clock::duration m_accumulator;
			/**
			 * \brief Gets the elapsed time since the timer was started.
			 * \return The elapsed time since the timer was started.
			*/
		Clock::duration ElapsedInternal();
	};
}
