/**
 * @author Christien Alden (34119981)
 * Mostly inspired by The Cherno's game engine entry point, hope to refactor this into
 * something that works better for our needs.
**/
#pragma once
#include "Logger.h"
#include "Application.h"
#include "AEngine/Script/ScriptEngine.h"

extern AEngine::Application* AEngine::CreateApplication();

#ifdef AE_PLATFORM_WINDOWS
	int main(int argc, char** argv)
	{
		AEngine::Logger::Init();
		AEngine::ScriptEngine::Init();

		AE_LOG_INFO("EntryPoint::main");
		auto app = AEngine::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}
#else
	#error "Platform not supported!"
#endif
