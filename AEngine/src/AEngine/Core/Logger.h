/**
 * \file
 * \author Christien Alden (34119981)
 * \details
 * Inspiration taken from The Cherno on YouTube as well as the spdlog wiki
*/
#pragma once
#include "Types.h"
#include <cstdlib>
#include <spdlog/spdlog.h>

namespace AEngine
{
		/**
		 * \class Logger
		 * \brief Provides logging functionality for the engine
		*/
	class Logger
	{
	public:
		using Handle = SharedPtr<spdlog::logger>;

			/**
			 * \brief Initialises logger to write to stdout
			**/
		static void Init();
			/**
			 * \brief Returns the internal logger
			 * \return The handle to the internal \ref Handle
			**/
		static Handle GetLogger();

		static Handle GetScriptLogger();

	private:
			/**
			 * \brief Internal logger object
			*/
		static Handle s_internal;
		static Handle s_scriptLogger;
	};
}

//--------------------------------------------------------------------------------
// Compile options
//--------------------------------------------------------------------------------
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
		#define AE_LOG_LUA_ERROR(...)	::AEngine::Logger::GetScriptLogger()->debug(__VA_ARGS__)
	#else
		#define AE_LOG_DEBUG(...)
		#define AE_LOG_LUA_ERROR(...)
	#endif

	#if AE_SHOW_INFO
		#define AE_LOG_INFO(...)		::AEngine::Logger::GetLogger()->info(__VA_ARGS__)
	#else
		#define AE_LOG_INFO(...)
	#endif

		#define AE_LOG_WARN(...)		::AEngine::Logger::GetLogger()->warn(__VA_ARGS__)
		#define AE_LOG_ERROR(...)		::AEngine::Logger::GetLogger()->error(__VA_ARGS__)
		#define AE_LOG_FATAL(...)	{	::AEngine::Logger::GetLogger()->critical(__VA_ARGS__); exit(1);	}
#else
	#define AE_LOG_TRACE(...)
	#define AE_LOG_DEBUG(...)
	#define AE_LOG_LUA_ERROR(...)
	#define AE_LOG_INFO(...)
	#define AE_LOG_WARN(...)
	#define AE_LOG_ERROR(...)
	#define AE_LOG_FATAL(...)			exit(1)
#endif
