/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <memory>
#include <cstdlib>
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
		m_activeScene = AEngine::SceneManager::Instance().LoadScene(std::make_unique<AEngine::Scene>("DemoScene"));
		if (!m_activeScene)
		{
			exit(1);
		}

		m_activeScene->LoadFromFile("assets/scenes/test.scene");

		AEngine::Scene *physicsTest = AEngine::SceneManager::Instance().LoadScene(std::make_unique<AEngine::Scene>("PhysicsTest"));
		if (!physicsTest)
		{
			exit(1);
		}
		physicsTest->LoadFromFile("assets/scenes/physicsTest.scene");


		if (!AEngine::SceneManager::Instance().SetActiveScene("DemoScene"))
		{
			exit(1);
		}

		m_activeScene->UseDebugCamera(true);
		AEngine::DebugCamera& debugCam = m_activeScene->GetDebugCamera();
		debugCam.SetFarPlane(1000.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);

		physicsTest->UseDebugCamera(true);
		AEngine::DebugCamera& nextDebugCam = physicsTest->GetDebugCamera();
		nextDebugCam.SetFarPlane(1000.0f);
		nextDebugCam.SetNearPlane(0.1f);
		nextDebugCam.SetFov(45.0f);
		nextDebugCam.SetYaw(-90.0f);

		m_activeScene->Start();
	}

	void OnDetach() override
	{
		m_activeScene->SaveToFile("assets/scenes/export.scene");
	}

	void OnUpdate(AEngine::TimeStep ts) override
	{
		AEngine::EventDispatcher e;
		// capture keyevent for testing
		e.Dispatch<AEngine::KeyPressed>([&, this](AEngine::KeyPressed& e) -> bool {
			switch (e.GetKey())
			{
			// case AEKey::ESCAPE:
				// AEngine::Application::Instance().Terminate();
				// break;
			case AEKey::F1:
				m_activeScene->TakeSnapshot();
				break;
			case AEKey::F2:
				m_activeScene->RestoreSnapshot();
				break;
			case AEKey::F3:
				m_activeScene->LoadFromFile("assets/scenes/test.scene");
				break;
			case AEKey::F4:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_LINE);
				break;
			case AEKey::F5:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_FILL);
				break;
			case AEKey::F6:
				if (AEngine::SceneManager::Instance().SetActiveScene("PhysicsTest"))
				{
					m_activeScene->Stop();
					m_activeScene = AEngine::SceneManager::Instance().GetActiveScene();
					m_activeScene->Start();
				}
				break;
			}
			return true;
			});

		m_activeScene->OnUpdate(ts);
	}

private:
	AEngine::Scene* m_activeScene;
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
