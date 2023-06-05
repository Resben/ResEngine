/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace AEngine
{
	Logger::Handle Logger::s_internal{ nullptr };
	Logger::Handle Logger::s_scriptLogger{ nullptr };

	void Logger::Init()
	{
		SharedPtr<spdlog::sinks::stdout_color_sink_mt> sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_internal = MakeShared<spdlog::logger>("Internal", sink);
		s_internal->set_pattern("%^[%T] : %v%$");
		s_internal->set_level(spdlog::level::trace);
		s_internal->flush_on(spdlog::level::trace);

		s_scriptLogger = MakeShared<spdlog::logger>("Script", sink);
		s_scriptLogger->set_pattern("%^[%T] Script : %v%$");
		s_scriptLogger->set_level(spdlog::level::debug);
		s_scriptLogger->flush_on(spdlog::level::debug);
	}

	Logger::Handle Logger::GetLogger()
	{
		return s_internal;
	}

	Logger::Handle Logger::GetScriptLogger()
	{
		return s_scriptLogger;
	}
}
