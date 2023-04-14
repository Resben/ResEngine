#include <glfw/glfw3.h>
#include "Application.h"
#include "Logger.h"
#include "../Resource/ShaderManager.h"
#include "InputManager.h"
#include "KeyCodes.h"

namespace AEngine
{
	Application *Application::s_instance = nullptr;

	Application::Application(const ApplicationProperties& props)
		: m_props(props)
	{
		// applicaton already created
		if (s_instance)
		{
			assert(false);
		}

		// set instance and initialise
		s_instance = this;
		init();
	}

	Application::~Application()
	{
		shutdown();
	}

	Application& Application::instance()
	{
		if (s_instance)
		{
			return *s_instance;
		}

		assert(false);
	}

	void Application::terminate()
	{
		m_running = false;
	}

	void Application::setLayer(Layer* layer)
	{
		m_layer = layer;
	}

	void Application::init()
	{
		AE_LOG_INFO("Application::Create");
		m_window = AEngine::Window::Create({ m_props.title, 1600, 900 });

		// load shaders
		AEngine::ShaderManager::Instance()->LoadShader(
			"flat", "assets/shaders/flat_vertex.shader", "assets/shaders/flat_fragment.shader");
		AEngine::ShaderManager::Instance()->LoadShader(
			"diffuse", "assets/shaders/diffuse_vertex.shader", "assets/shaders/diffuse_fragment.shader");

		// change to render facade
		glfwSetKeyCallback(static_cast<GLFWwindow*>(m_window->GetNative()), AEngine::InputManager::KeyCallback);
	}

	void Application::shutdown()
	{
		AE_LOG_INFO("Runtime::Terminate");

		// shutdown render
		// terminate window
		// etc.

		AEngine::ShaderManager::Instance()->Clear();
	}

	// must be called externally
	void Application::run()
	{
		// start clock
		m_clock.start();

		AE_LOG_INFO("Runtime::MainLoop");
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.update();

			// update layers
			m_layer->onUpdate(dt);

			// render frame
			m_window->OnUpdate();
		}
	}
}

