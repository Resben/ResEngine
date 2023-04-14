#pragma once
#include "Logger.h"
#include "Application.h"

extern AEngine::Application* AEngine::createApplication();

int main(int argc, char** argv)
{
	AEngine::Logger::Init();

	auto app = AEngine::createApplication();
	app->mainLoop();
	delete app;

	return 0;
}
