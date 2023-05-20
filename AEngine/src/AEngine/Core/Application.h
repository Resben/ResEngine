/**
 * \file
 * \author Christien Alden (34119981)
**/
#pragma once
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Render/GraphicsAPI.h"
#include "LayerStack.h"
#include "Timer.h"
#include "Types.h"
#include <string>

// for friend statement
extern int main(int argc, char** argv);

namespace AEngine
{
	class Window;

		/**
		 * \struct ApplicationProps
		 * \brief Holds the application properties
		**/
	struct ApplicationProps
	{
			/// \brief The title bar text
		std::string title = "AEngine Demo";
			/// \brief The working directory of the application
		std::string workingDir;
	};

		/**
		 * \class Application
		 * \brief Encapsulates the application runtime
		**/
	class Application
	{
	public:
			/**
			 * \brief Creates an application
			 * \param[in] props The properties of the application
			*/
		Application(const ApplicationProps& props);
			/**
			 * \brief Destroys the application
			*/
		virtual ~Application();
			/**
			 * \brief Returns the instance of the application
			 * \return The instance of the application
			*/
		static Application& Instance();
			/**
			 * \brief Terminates the application runtime
			 * \note This is called internally when no windows are open
			*/
		void Terminate();
			/**
			 * \brief Pushes a layer onto the layer stack
			 * \param[in] layer The layer to push
			 * \note The layer is moved into the stack
			 * \note The layer is deleted when the stack is destroyed
			*/
		void PushLayer(UniquePtr<Layer> layer);
			/**
			 * \brief Returns the graphics API
			 * \return The graphics API
			 * \note This is a temporary solution
			 * \todo Remove this
			*/
		GraphicsAPI& Graphics();
			/**
			 * \brief Returns the window size
			 * \return The window size
			 * \note This is a temporary solution
			 * \todo Remove this
			*/
		Math::vec2 GetWindowSize();

	private:
			/**
			 * \brief Initialises the application specific modules.
			 * \note This is called in the constructor
			*/
		void Init();
			/**
			 * \brief Runs the application runtime
			*/
		void Run();
			/**
			 * \brief Shuts down the application specific modules
			 * \note This is called in the destructor
			*/
		void Shutdown();
			/**
			 * \brief Handles a window closed event
			 * \param[in] e The window closed event
			 * \retval true if the event should be popped of the event queue
			 * \retval false if the event should not be popped of the event queue
			*/
		bool OnWindowClose(WindowClosed& e);
			/**
			 * \brief Handles a window resized event
			 * \param[in] e The window resized event
			 * \retval true if the event should be popped of the event queue
			 * \retval false if the event should not be popped of the event queue
			*/
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
	Application* CreateApplication(ApplicationProps& props);
}
