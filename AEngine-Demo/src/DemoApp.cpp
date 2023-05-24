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
		// load scenes
		AEngine::Scene *testScene = AEngine::SceneManager::LoadFromFile("assets/scenes/test.scene");
		AEngine::Scene *physicsScene = AEngine::SceneManager::LoadFromFile("assets/scenes/physicsTest.scene");
		if (!testScene || !physicsScene)
		{
			exit(1);
		}

		// set active scene and debug camerae
		AEngine::SceneManager::SetActiveScene("Test Scene");
		AEngine::Scene::UseDebugCamera(true);
		AEngine::DebugCamera& debugCam = AEngine::Scene::GetDebugCamera();
		debugCam.SetFarPlane(1000.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);
		AEngine::SceneManager::GetActiveScene()->Start();
	}

	void OnDetach() override
	{
		AEngine::SceneManager::SaveActiveToFile("assets/scenes/export.scene");
	}

	void OnUpdate(AEngine::TimeStep ts) override
	{
		AEngine::EventDispatcher e;
		// capture keyevent for testing
		e.Dispatch<AEngine::KeyPressed>([&, this](AEngine::KeyPressed& e) -> bool {
			switch (e.GetKey())
			{
			case AEKey::F1:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_LINE);
				break;
			case AEKey::F2:
				AEngine::Application::Instance().Graphics().PolygonMode(AEngine::AE_TYPES::AE_FILL);
				break;
			}
			return true;
			});

		AEngine::SceneManager::GetActiveScene()->OnUpdate(ts);
	}
};

class DemoApp : public AEngine::Application
{
public:
	DemoApp(AEngine::Application::Properties props)
		: Application(props)
	{
		PushLayer(std::make_unique<DemoLayer>("Test Layer"));
	}
};

AEngine::Application* AEngine::CreateApplication(AEngine::Application::Properties& props)
{
	props.title = "AEngine Demo";
	return new DemoApp(props);
}
