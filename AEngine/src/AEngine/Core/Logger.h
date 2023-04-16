/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Logger for internal debugging
 *
 * Inspiration taken from The Cherno on YouTube as well as the spdlog wiki
**/
#pragma once
#include <spdlog/spdlog.h>
#include <cstdlib>

namespace AEngine
{
	class Logger
	{
	public:
			/**
			 * @brief Initialises logger to write to stdout
			 * @return void
			**/
		static void Init();
			/**
			 * @brief Returns a shared pointer to logger
			 * @return std::shared_ptr<spdlog::logger>
			**/
		static std::shared_ptr<spdlog::logger> GetLogger();

	private:
			/// Internal logger handle
		static std::shared_ptr<spdlog::logger> s_internal;
	};
}

// Enable and Disable Log Levels
#define AE_SHOW_TRACE	0
#define AE_SHOW_DEBUG	1
#define AE_SHOW_INFO	1

#ifdef AE_DEBUG
	#if AE_SHOW_TRACE
		#define AE_LOG_TRACE(...)		::AEngine::Logger::GetLogger()->trace(__VA_ARGS__)
	#else
		#define AE_LOG_TRACE(...)
	#endif

	#if AE_SHOW_DEBUG
		#define AE_LOG_DEBUG(...)		::AEngine::Logger::GetLogger()->debug(__VA_ARGS__)
	#else
		#define AE_LOG_DEBUG(...)
	#endif

	#if AE_SHOW_INFO
		#define AE_LOG_INFO(...)		::AEngine::Logger::GetLogger()->info(__VA_ARGS__)
	#else
		#define AE_LOG_INFO(...)
	#endif

		#define AE_LOG_WARN(...)		::AEngine::Logger::GetLogger()->warn(__VA_ARGS__)
		#define AE_LOG_ERROR(...)		::AEngine::Logger::GetLogger()->error(__VA_ARGS__)
		#define AE_LOG_FATAL(...)		::AEngine::Logger::GetLogger()->critical(__VA_ARGS__); exit(1)
#else
	#define AE_LOG_TRACE(...)
	#define AE_LOG_DEBUG(...)
	#define AE_LOG_INFO(...)
	#define AE_LOG_WARN(...)
	#define AE_LOG_ERROR(...)
	#define AE_LOG_FATAL(...)			exit(1)
#endif
