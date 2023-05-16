#include "Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Events/EventQueue.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Render/Model.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Texture.h"

namespace AEngine
{
	Application *Application::s_instance = nullptr;

	Application::Application(const ApplicationProps& props)
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
		AE_LOG_INFO("Application::Destructor");
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

	void Application::PushLayer(UniquePtr<Layer> layer)
	{
		m_layers.PushLayer(std::move(layer));
	}

	GraphicsAPI& Application::Graphics()
	{
		return *m_cmds;
	}

	Math::vec2 Application::GetWindowSize()
	{
		return m_window->GetSize();
	}

	void Application::Init()
	{
		AE_LOG_INFO("Application::Init");
		m_cmds = AEngine::GraphicsAPI::Create(GraphicsType::OPENGL);
		m_window = AEngine::Window::Create({ m_props.title, 1600, 900 });
		m_running = true;
		m_minimised = false;

		m_cmds->SetClearColor(Math::vec4(255.0, 255.0, 255.0, 255.0));
		m_cmds->EnableDepth(true);
	}

	bool Application::OnWindowClose(WindowClosed& e)
	{
		Terminate();
		return true;
	}

	bool Application::OnWindowResize(WindowResized& e)
	{
		unsigned int width = e.GetWidth();
		unsigned int height = e.GetHeight();
		m_minimised = (width == 0 && height == 0) ? true : false;
		m_cmds->SetViewport(0, 0, width, height);
		return true;
	}

	void Application::Shutdown()
	{
		AE_LOG_INFO("Applicaton::Shutdown");

		// shutdown render
		// terminate window
		// etc.

		m_layers.Clear();
		AEngine::AssetManager<Model>::Instance().Clear();
		AEngine::AssetManager<Shader>::Instance().Clear();
		AEngine::AssetManager<Texture>::Instance().Clear();
	}

	// must be called externally
	void Application::Run()
	{
		AE_LOG_INFO("Application::Run");

		// start clock
		m_clock.Start();
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.GetDelta();

			// poll for application events
			EventDispatcher e;
			e.Dispatch<WindowClosed>(AE_EVENT_FN(&Application::OnWindowClose));
			e.Dispatch<WindowResized>(AE_EVENT_FN(&Application::OnWindowResize));

			// update layers
			for(auto it = m_layers.begin(); it != m_layers.end(); ++it)
			{
				(*it)->OnUpdate(dt);
			}

			// render frame
			EventQueue::Instance().Clear();
			m_window->OnUpdate();
		}
	}
}
