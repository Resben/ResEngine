/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include <memory>
#include "Input.h"
#include "Layer.h"
#include "Timer.h"
#include "TimeStep.h"
#include "Window.h"
#include "AEngine/Events/ApplicationEvent.h"
#include <AEngine/Render/GraphicsAPI.h>

// for friend statement
extern int main(int argc, char** argv);

namespace AEngine
{
		/**
		 * @struct ApplicationProps
		 * @brief Holds the application properties
		**/
	struct ApplicationProps
	{
		std::string title = "AEngine Demo";
	};

		/**
		 * @class Application
		 * @brief Encapsulates the application runtime
		**/
	class Application
	{
	public:
		Application(const ApplicationProps& props);
		virtual ~Application();
		static Application& Instance();
		void Terminate();
		void SetLayer(Layer* layer);
		InputQuery& Input();
		GraphicsAPI& Graphics();


	private:
		void Init();
		void Run();
		void Shutdown();

		bool OnWindowClose(WindowClosed& e);
		bool OnWindowResize(WindowResized& e);

	private:
		// general
		static Application* s_instance;
		ApplicationProps m_props;
		std::unique_ptr<Window> m_window{ nullptr };
		std::unique_ptr<GraphicsAPI> m_cmds{ nullptr };

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
