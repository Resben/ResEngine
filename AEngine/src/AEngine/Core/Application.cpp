#include "Application.h"
#include "AEngine/Core/Logger.h"
#include "../Resource/ShaderManager.h"

namespace AEngine
{
	Application *Application::s_instance = nullptr;

	Application::Application(const ApplicationProperties& props)
		: m_props(props)
	{
		// applicaton already created
		if (s_instance)
		{
			AE_LOG_CRITICAL("Application::Error -> Already Created");
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

		AE_LOG_CRITICAL("Application::Instance -> None present");
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
		AE_LOG_INFO("Application::Initialise");
		m_window = AEngine::Window::Create({ m_props.title, 1600, 900 });
	}

	void Application::onWindowClose()
	{
		terminate();
	}

	void Application::shutdown()
	{
		AE_LOG_INFO("Applicaton::Shutdown");

		// shutdown render
		// terminate window
		// etc.

		AEngine::ShaderManager::Instance()->Clear();
	}

	// must be called externally
	void Application::run()
	{
		AE_LOG_INFO("Application::Run");

		// start clock
		m_clock.start();
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.update();

			// poll for application events
			// check each event in Event Queue for correct type and execute
			
			// update layers
			//m_layer->onUpdate(dt);

			// render frame
			m_window->OnUpdate();
		}
	}
}
