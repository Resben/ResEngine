#include "Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/ShaderManager.h"
#include "AEngine/Resource/ModelManager.h"
#include "AEngine/Events/EventQueue.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"

#include "AEngine/Scene/DebugCamera.h"
#include "AEngine/Render/Renderer.h"
#include "AEngine/Scene/Components.h"


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

	void Application::SetLayer(Layer* layer)
	{
		m_layer = layer;
	}

	InputQuery& Application::Input()
	{
		return m_window->GetInput();
	}

	GraphicsAPI& Application::Graphics()
	{
		return *m_cmds;
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

		ModelManager::Instance()->LoadModel("assets/testobject/untitled.obj");
		std::shared_ptr<Shader> shader = ShaderManager::Instance()->LoadShader("simple", "assets/shaders/simple.shader");
		DebugCamera debugCam;
		debugCam.SetFarPlane(100.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);

		// start clock
		m_clock.Start();
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.Update();

			debugCam.OnUpdate(dt);

			// poll for application events
			EventDispatcher e;
			e.Dispatch<WindowClosed>(AE_EVENT_FN(&Application::OnWindowClose));
			e.Dispatch<WindowResized>(AE_EVENT_FN(&Application::OnWindowResize));

			// update layers
			//m_layer->onUpdate(dt);

			// test render
			Renderer::Instance()->SetProjection(debugCam.GetProjectionViewMatrix());
			Renderer::Instance()->Submit(*ModelManager::Instance()->GetModel("untitled.obj"), *shader, Math::mat4(1.0f));

			// remove me
			if (Input().IsKeyPressed(AEKey::ESCAPE))
			{
				Terminate();
			}

			// render frame
			EventQueue::Instance().Clear();
			m_window->OnUpdate();
		}
	}
}
