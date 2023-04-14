#pragma once
#include <memory>
#include "Timer.h"
#include "TimeStep.h"
#include "Window.h"

extern int main(int argc, char** argv);

namespace AEngine
{
	struct ApplicationProperties
	{
		std::string title = "AEngine Demo";
	};

	class Application
	{
	public:
		Application(const ApplicationProperties& props);
		static Application& instance();

	private:
		virtual ~Application();

		void init();
		void mainLoop();
		void shutdown();

		//void onWindowResize();

	private:
		static Application* s_instance;
		ApplicationProperties m_props;
		std::unique_ptr<Window> m_window{ nullptr };
		Timer m_clock{};
		TimeStep m_frameTime{};
		
		// to create the applicaton outside of the class
		friend int ::main(int argc, char** argv);
	};

	// define in client as this is called in EntryPoint.h
	Application* createApplication();
}
