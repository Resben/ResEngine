#include "Application.h"
#include "AEngine/Core/Logger.h"
#include "../Resource/ShaderManager.h"
#include "AEngine/Events/EventQueue.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"

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

	bool Application::OnWindowClose(WindowClosed& e)
	{
		AE_LOG_TRACE("{}", e.GetName());
		terminate();
		return true;
	}

	bool Application::OnWindowResize(WindowResized& e)
	{
		AE_LOG_TRACE("{}: {} - {}", e.GetName(), e.GetWidth(), e.GetHeight());
		return true;
	}

	void Application::shutdown()
	{
		AE_LOG_INFO("Applicaton::Shutdown");

		// shutdown render
		// terminate window
		// etc.

		AEngine::ShaderManager::Instance()->Clear();
	}

	bool Application::keyPressedCallback(KeyPressed& e)
	{
		AE_LOG_INFO("KeyPressed -> {}", static_cast<int>(e.GetKey()));
		return true;
	}

	bool Application::keyTypedCallback(KeyTyped& e)
	{
		AE_LOG_INFO("KeyTyped -> {}", static_cast<char>(e.GetKey()));
		return true;
	}

	bool Application::onMouseMove(MouseMoved& e)
	{
		AE_LOG_INFO("MouseMoved -> {} - {}", e.GetX(), e.GetY());
		return true;
	}

	bool Application::onButtonPressed(MouseButtonPressed& e)
	{
		AE_LOG_INFO("MousePressed -> {}", static_cast<int>(e.GetButton()));
		return true;
	}

	bool Application::onScroll(MouseScrolled& e)
	{
		AE_LOG_INFO("MouseScrolled -> {} - {}", e.GetScrollX(), e.GetScrollY());
		return true;
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
			EventDispatcher e;
			e.Dispatch<WindowClosed>(AE_EVENT_FN(&Application::OnWindowClose));
			e.Dispatch<WindowResized>(AE_EVENT_FN(&Application::OnWindowResize));
			e.Dispatch<KeyTyped>(AE_EVENT_FN(&Application::keyTypedCallback));
			e.Dispatch<KeyPressed>(AE_EVENT_FN(&Application::keyPressedCallback));
			e.Dispatch<MouseMoved>(AE_EVENT_FN(&Application::onMouseMove));
			e.Dispatch<MouseButtonPressed>(AE_EVENT_FN(&Application::onButtonPressed));
			e.Dispatch<MouseScrolled>(AE_EVENT_FN(&Application::onScroll));

			// update layers
			//m_layer->onUpdate(dt);

			// render frame
			m_window->OnUpdate();
		}
	}
}
