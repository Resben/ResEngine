/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "AEngine/Core/Logger.h"
#include "AEngine/Events/EventHandler.h"
#include "AEngine/Input/InputBuffer.h"
#include "AEngine/Render/RenderCommand.h"
#include "AEngine/Render/ResourceAPI.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Scene/SceneManager.h"
#include "AEngine/Render/RenderPipeline.h"
#include "AEngine/Render/UIRenderCommand.h"
#include "Application.h"
#include "TimeStep.h"
#include "Window.h"

namespace AEngine
{
	// forward declarations
	class Model;
	class Script;
	class Shader;
	class Texture;

	// static initialisation
	Application *Application::s_instance = nullptr;

	Application::Application(const Properties& properties)
		: m_properties{ properties },
		  m_window{ nullptr }, m_running{ true },
		  m_minimised{ false }, m_layer{},
		  m_clock{}
	{
		// if applicaton already created
		if (s_instance)
		{
			AE_LOG_FATAL("Application::Constructor::Fatal -> Application can not have multiple instances");
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

		AE_LOG_FATAL("Application::Instance::Fatal -> None present");
	}

	void Application::Terminate()
	{
		m_running = false;
	}

	void Application::SetLayer(UniquePtr<Layer> layer)
	{
		if (m_layer)
		{
			m_layer->OnDetach();
		}

		m_layer = std::move(layer);
		m_layer->OnAttach();
	}

	Window* Application::GetWindow() const
	{
		return m_window.get();
	}

	InputBuffer &Application::GetInput()
	{
		return m_input;
	}

	void Application::Init()
	{
		AE_LOG_INFO("Application::Init");
		ResourceAPI::Initialise(ModelLoaderLibrary::Assimp);
		RenderCommand::Initialise(RenderLibrary::OpenGL);
		m_window = AEngine::Window::Create({ m_properties.title, 1600, 900 });
		EditorProperties props;
		m_editor.Init(m_window.get(), props);

		// setup application event callbacks
		// using priority level 0 to give application layer priority
		m_window->RegisterEventHandler<WindowClosed>(2, AE_EVENT_FN(&Application::OnWindowClose));
		m_window->RegisterEventHandler<WindowResized>(2, AE_EVENT_FN(&Application::OnWindowResize));

		// setup input callbacks
		// using priority level 1 to give gui layer priority
		m_window->RegisterEventHandler<KeyPressed>(2, [this](KeyPressed& e) -> bool {
			m_input.SetKeyState(e.GetKey(), AEInputState::Pressed);
			return false;
		});
		m_window->RegisterEventHandler<KeyReleased>(2, [this](KeyReleased& e) -> bool {
			m_input.SetKeyState(e.GetKey(), AEInputState::Released);
			return false;
		});
		m_window->RegisterEventHandler<KeyRepeated>(2, [this](KeyRepeated& e) -> bool {
			m_input.SetKeyState(e.GetKey(), AEInputState::Repeated);
			return false;
		});
		m_window->RegisterEventHandler<MousePressed>(2, [this](MousePressed& e) -> bool {
			m_input.SetMouseButtonState(e.GetButton(), AEInputState::Pressed);
			return false;
		});
		m_window->RegisterEventHandler<MouseReleased>(2, [this](MouseReleased& e) -> bool {
			m_input.SetMouseButtonState(e.GetButton(), AEInputState::Released);
			return false;
		});
		m_window->RegisterEventHandler<MouseRepeated>(2, [this](MouseRepeated& e) -> bool {
			m_input.SetMouseButtonState(e.GetButton(), AEInputState::Repeated);
			return false;
		});
		m_window->RegisterEventHandler<MouseMoved>(2, [this](MouseMoved& e) -> bool {
			m_input.SetMouseState(e.GetPos(), e.GetDelta());
			return false;
		});
		m_window->RegisterEventHandler<MouseScrolled>(2, [this](MouseScrolled& e) -> bool {
			m_input.SetMouseScroll(e.GetScroll());
			return false;
		});

		// setup default render state
		RenderCommand::SetClearColor(Math::vec4{ 255.0f, 255.0f, 255.0f, 255.0f });
		RenderCommand::EnableDepthTest(true);

		// set default cursor state
		m_window->ShowCursor(true);
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
		RenderCommand::SetViewport(0, 0, width, height);

		/// \todo Find a better solution than this for handling resize events
		const std::vector<std::string> scenes = SceneManager::GetSceneIdents();
		for (const std::string& ident : scenes)
		{
			SceneManager::GetScene(ident)->OnViewportResize(width, height);
		}

		RenderPipeline::Instance().OnWindowResize({e.GetWidth(), e.GetHeight()});

		return false;
	}

	void Application::Shutdown()
	{
		m_editor.Shutdown();
		AE_LOG_INFO("Applicaton::Shutdown");
		m_layer->OnDetach();
		SceneManager().UnloadAllScenes();
		AEngine::AssetManager<Model>::Instance().Clear();
		AEngine::AssetManager<Script>::Instance().Clear();
		AEngine::AssetManager<Shader>::Instance().Clear();
		AEngine::AssetManager<Texture>::Instance().Clear();
		AEngine::UIRenderCommand::Teardown();
	}

	// must be called externally
	void Application::Run()
	{
		AE_LOG_INFO("Application::Run");

		m_clock.Start();
		while (m_running)
		{
			TimeStep dt = m_clock.GetDelta();

			// update the editor
			m_editor.CreateNewFrame();

			// if the window is minimised, don't update the layers
			// the engine will still poll input and swap the buffers
			if (!m_minimised)
			{
				m_layer->OnUpdate(dt);
			}

			// update input and swap buffers
			m_editor.Update();
			m_editor.Render();
			m_window->OnUpdate();
		}
	}
}
