#pragma once
#include <memory>
#include "Layer.h"
#include "Timer.h"
#include "TimeStep.h"
#include "Window.h"
#include "AEngine/Events/ApplicationEvent.h"

// for friend statement
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
		static Application& Instance();
		void Terminate();
		void SetLayer(Layer* layer);

	private:
		void Init();
		void Run();
		void Shutdown();

		bool OnWindowClose(WindowClosed& e);
		bool OnWindowResize(WindowResized& e);

	private:
		// general
		static Application* s_instance;
		ApplicationProperties m_props;
		std::unique_ptr<Window> m_window{ nullptr };

		// runtime
		Layer* m_layer{ nullptr };
		Timer m_clock{};
		bool m_running;
		bool m_minimised;
		
		// to create the applicaton outside of the class
		friend int ::main(int argc, char** argv);
	};

	// define in client as this is called in EntryPoint.h
	Application* CreateApplication();
}
