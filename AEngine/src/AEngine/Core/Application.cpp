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
			AE_LOG_FATAL("Application::Error -> Already Created");
		}

		// set instance and initialise
		s_instance = this;
		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	Application& Application::Instance()
	{
		if (s_instance)
		{
			return *s_instance;
		}

		AE_LOG_FATAL("Application::Instance -> None present");
	}

	void Application::Terminate()
	{
		m_running = false;
	}

	void Application::SetLayer(Layer* layer)
	{
		m_layer = layer;
	}

	InputQuery& Application::Input()
	{
		return m_window->GetInput();
	}

	void Application::Init()
	{
		AE_LOG_INFO("Application::Initialise");
		m_window = AEngine::Window::Create({ m_props.title, 1600, 900 });
		m_running = true;
		m_minimised = false;
	}

	bool Application::OnWindowClose(WindowClosed& e)
	{
		AE_LOG_TRACE("{}", e.GetName());
		Terminate();
		return true;
	}

	bool Application::OnWindowResize(WindowResized& e)
	{
		unsigned int width = e.GetWidth();
		unsigned int height = e.GetHeight();
		m_minimised = (width == 0 && height == 0) ? true : false;
		AE_LOG_TRACE("{}: {} - {}", e.GetName(), width, height);
		return true;
	}

	void Application::Shutdown()
	{
		AE_LOG_INFO("Applicaton::Shutdown");

		// shutdown render
		// terminate window
		// etc.

		AEngine::ShaderManager::Instance()->Clear();
	}

	// must be called externally
	void Application::Run()
	{
		AE_LOG_INFO("Application::Run");

		// start clock
		m_clock.start();
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.update();

			// poll for application events
			EventDispatcher e;
			e.Dispatch<WindowClosed>(AE_EVENT_FN(&Application::OnWindowClose));
			e.Dispatch<WindowResized>(AE_EVENT_FN(&Application::OnWindowResize));

			// testing callbacks
			e.Dispatch<KeyPressed>([](KeyPressed& e) {
				AE_LOG_TRACE("{} -> {}", e.GetName(), static_cast<int>(e.GetKey()));
				return true;
			});
			
			e.Dispatch<KeyReleased>([](KeyReleased& e) {
				AE_LOG_TRACE("{} -> {}", e.GetName(), static_cast<int>(e.GetKey()));
				return true;
			});

			e.Dispatch<KeyTyped>([](KeyTyped& e) {
				AE_LOG_TRACE("{} -> {}", e.GetName(), static_cast<char>(e.GetKey()));
				return true;
			});

			e.Dispatch<MouseMoved>([](MouseMoved& e) {
				AE_LOG_TRACE("{} -> {} - {}", e.GetName(), e.GetX(), e.GetY());
				return true;
			});
			
			e.Dispatch<MouseButtonPressed>([](MouseButtonPressed& e) {
				AE_LOG_TRACE("{} -> {}", e.GetName(), static_cast<int>(e.GetButton()));
				return true;
			});

			e.Dispatch<MouseButtonReleased>([](MouseButtonReleased& e) {
				AE_LOG_TRACE("{} -> {}", e.GetName(), static_cast<int>(e.GetButton()));
				return true;
			});

			e.Dispatch<MouseScrolled>([](MouseScrolled& e) {
				AE_LOG_TRACE("{} -> {} - {}", e.GetName(), e.GetX(), e.GetY());
				return true;
			});

			// update layers
			//m_layer->onUpdate(dt);

			// render frame
			EventQueue::Instance().Clear();
			m_window->OnUpdate();
		}
	}
}
