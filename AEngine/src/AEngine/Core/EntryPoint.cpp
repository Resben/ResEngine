/**
 * @author Christien Alden (34119981)
 * Mostly inspired by The Cherno's game engine entry point, hope to refactor this into 
 * something that works better for our needs.
**/
#pragma once
#include "Logger.h"
#include "Application.h"

extern AEngine::Application* AEngine::createApplication();

#ifdef AE_PLATFORM_WINDOWS
	int main(int argc, char** argv)
	{
		AEngine::Logger::Init();

		auto app = AEngine::createApplication();
		app->run();
		delete app;

		return 0;
	}
#else
	#error "Platform not supported!"
#endif
