#include "Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Events/EventQueue.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"
#include "AEngine/Scene/Scene.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/DebugCamera.h"
#include "AEngine/Scene/Entity.h"

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

		AEngine::AssetManager<Model>::Instance().Clear();
		AEngine::AssetManager<Shader>::Instance().Clear();
		AEngine::AssetManager<Texture>::Instance().Clear();
	}

	// must be called externally
	void Application::Run()
	{
		AE_LOG_INFO("Application::Run");

		std::shared_ptr<Scene> testScene = std::make_shared<Scene>("Test Scene");
		testScene->LoadFromFile("assets/scenes/test.scene");
		// testScene->LoadFromFile("assets/scenes/export.scene");
		testScene->UseDebugCamera(true);
		DebugCamera& debugCam = testScene->GetDebugCamera();
		debugCam.SetFarPlane(100.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);

		testScene->Init();
		testScene->Start();

		// start clock
		m_clock.Start();
		while (m_running)
		{
			// update frame time
			TimeStep dt = m_clock.Update();

			// poll for application events
			EventDispatcher e;
			e.Dispatch<WindowClosed>(AE_EVENT_FN(&Application::OnWindowClose));
			e.Dispatch<WindowResized>(AE_EVENT_FN(&Application::OnWindowResize));

			// update layers
			//m_layer->onUpdate(dt);
			testScene->OnUpdate(dt);

			// capture keyevent for testing
			e.Dispatch<KeyPressed>([&, this](KeyPressed& e) -> bool {
				switch (e.GetKey())
				{
				case AEKey::ESCAPE:
					this->Terminate();
					break;
				case AEKey::F1:
					testScene->TakeSnapshot();
					break;
				case AEKey::F2:
					testScene->RestoreSnapshot();
					break;
				case AEKey::C:
					Entity entity = testScene->GetEntity("Box1");
					TransformComponent* trans = entity.GetComponent<TransformComponent>();
					trans->scale *= Math::vec3(1.5, 1.5, 1.5);
					break;
				}
				return true;
			});

			// render frame
			EventQueue::Instance().Clear();
			m_window->OnUpdate();
		}

		testScene->SaveToFile("assets/scenes/export.scene");
	}
}
