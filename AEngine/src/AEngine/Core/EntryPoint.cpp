/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Entry point for the application
 * \details
 * Mostly inspired by The Cherno's game engine entry point, hope to refactor this into
 * something that works better for our needs.
**/
#pragma once
#include "AEngine/Script/ScriptEngine.h"
#include "Application.h"
#include "Logger.h"

extern AEngine::Application* AEngine::CreateApplication(AEngine::Application::Properties&);

#ifdef AE_PLATFORM_WINDOWS
	int main(int argc, char** argv)
	{
		AEngine::Logger::Init();
		AEngine::ScriptEngine::Init();

		AEngine::Application::Properties props;
		props.workingDir = argv[0];

		AE_LOG_INFO("EntryPoint::main");
		auto app = AEngine::CreateApplication(props);
		app->Run();
		delete app;

		return 0;
	}
#else
	#error "Platform not supported!"
#endif
