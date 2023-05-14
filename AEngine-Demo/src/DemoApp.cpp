/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <memory>
#include <AEngine.h>

class DemoLayer : public AEngine::Layer
{
public:
	DemoLayer(const std::string& ident)
		: AEngine::Layer(ident)
	{
	}

	void OnAttach() override
	{
		m_scene = std::make_shared<AEngine::Scene>("DemoScene");
		m_scene->LoadFromFile("assets/scenes/test.scene");

		m_scene->UseDebugCamera(true);
		AEngine::DebugCamera& debugCam = m_scene->GetDebugCamera();
		debugCam.SetFarPlane(1000.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);

		m_scene->Start();
	}

	void OnDetach() override
	{
		m_scene->SaveToFile("assets/scenes/export.scene");
	}

	void OnUpdate(AEngine::TimeStep ts) override
	{
		AEngine::EventDispatcher e;
		// capture keyevent for testing
		e.Dispatch<AEngine::KeyPressed>([&, this](AEngine::KeyPressed& e) -> bool {
			switch (e.GetKey())
			{
			case AEKey::ESCAPE:
				AEngine::Application::Instance().Terminate();
				break;
			case AEKey::F1:
				m_scene->TakeSnapshot();
				break;
			case AEKey::F2:
				m_scene->RestoreSnapshot();
				break;
			case AEKey::F3:
				m_scene->LoadFromFile("assets/scenes/test.scene");
				break;
			case AEKey::F4:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_LINE);
				break;
			case AEKey::F5:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_FILL);
				break;

			}
			return true;
			});

		m_scene->OnUpdate(ts);
	}

private:
	std::shared_ptr<AEngine::Scene> m_scene;
};

class DemoApp : public AEngine::Application
{
public:
	DemoApp(AEngine::ApplicationProps props)
		: AEngine::Application(props)
	{
		PushLayer(std::make_unique<DemoLayer>("Test Layer"));
	}
};

AEngine::Application* AEngine::CreateApplication()
{
	ApplicationProps props;
	props.title = "AEngine Demo from outside engine";

	return new DemoApp(props);
}
