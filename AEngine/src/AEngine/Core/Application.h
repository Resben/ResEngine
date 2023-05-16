/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include "Input.h"
#include "Types.h"
#include "LayerStack.h"
#include "Timer.h"
#include "TimeStep.h"
#include "Window.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Render/GraphicsAPI.h"

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
		void PushLayer(UniquePtr<Layer> layer);
		GraphicsAPI& Graphics();
		Math::vec2 GetWindowSize();

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
		UniquePtr<Window> m_window{ nullptr };
		UniquePtr<GraphicsAPI> m_cmds{ nullptr };

		// runtime
		LayerStack m_layers;
		Timer m_clock{};
		bool m_running;
		bool m_minimised;

		// to create the applicaton outside of the class
		friend int ::main(int argc, char** argv);
	};

	// define in client as this is called in EntryPoint.h
	Application* CreateApplication();
}
