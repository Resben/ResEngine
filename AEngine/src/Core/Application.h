#pragma once
#include <memory>
#include "Layer.h"
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
		virtual ~Application();
		static Application& instance();
		void terminate();
		void setLayer(Layer* layer);

	private:
		void init();
		void run();
		void shutdown();

		//void onWindowResize();

	private:
		// general
		static Application* s_instance;
		ApplicationProperties m_props;
		std::unique_ptr<Window> m_window{ nullptr };

		// runtime
		Layer* m_layer{ nullptr };
		Timer m_clock{};
		bool m_running;
		
		// to create the applicaton outside of the class
		friend int ::main(int argc, char** argv);
	};

	// define in client as this is called in EntryPoint.h
	Application* createApplication();
}
