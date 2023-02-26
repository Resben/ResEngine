/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Logger for internal debugging
**/
#include "AEngine/Core/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace AEngine
{
	std::shared_ptr<spdlog::logger> Logger::s_internal;

	void Logger::Init()
	{
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_internal = std::make_shared<spdlog::logger>("Internal", sink);
		s_internal->set_pattern("%^[%T] : %v%$");
		s_internal->set_level(spdlog::level::trace);
		s_internal->flush_on(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Logger::GetLogger()
	{
		return s_internal;
	}
}
