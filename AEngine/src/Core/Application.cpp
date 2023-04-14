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
	void Application::mainLoop()
	{
		// start clock
		m_clock.start();

		// run scenes
		AE_LOG_INFO("Runtime::MainLoop");
		while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_window->GetNative())))
		{
			// update frame time
			m_frameTime = m_clock.update();
			
			// do all our shit
			if (AEngine::InputManager::IsKeyNoRepeat(AE_KEY_ESCAPE))
				glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_window->GetNative()), true);

			// remove me
			AE_LOG_INFO("Frame time = {}", m_frameTime);

			// render frame
			m_window->OnUpdate();
		}

		shutdown();
	}
}

