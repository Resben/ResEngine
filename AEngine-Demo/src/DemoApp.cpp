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
		AEngine::Scene *physicsTestScene = AEngine::SceneManager::LoadFromFile("assets/scenes/physicsTest.scene");
		AEngine::Scene *renderTestScene = AEngine::SceneManager::LoadFromFile("assets/scenes/renderTest.scene");
		if (!testScene | !physicsTestScene | !renderTestScene)
		{
			exit(1);
		}

		// set active scene and debug camerae
		// AEngine::SceneManager::SetActiveScene("Test Scene");
		AEngine::SceneManager::SetActiveScene("Physics Test Scene");
		AEngine::Scene::UseDebugCamera(false);
		AEngine::DebugCamera& debugCam = AEngine::Scene::GetDebugCamera();
		debugCam.SetFarPlane(1000.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);
		AEngine::SceneManager::GetActiveScene()->Start();

		// set default camera
		AEngine::CameraComponent* camComp = AEngine::SceneManager::GetActiveScene()->GetEntity("Player").GetComponent<AEngine::CameraComponent>();
		AEngine::SceneManager::GetActiveScene()->SetActiveCamera(&camComp->camera);
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
				AEngine::RenderCommand::PolygonMode(AEngine::PolygonFace::FrontAndBack, AEngine::PolygonDraw::Fill);
				break;
			case AEKey::F2:
				AEngine::RenderCommand::PolygonMode(AEngine::PolygonFace::FrontAndBack, AEngine::PolygonDraw::Line);
				break;
			case AEKey::P:
				// toggle scene running
				if (AEngine::SceneManager::GetActiveScene()->IsRunning())
				{
					AEngine::SceneManager::GetActiveScene()->Stop();
					AEngine::Application::Instance().GetWindow()->ShowCursor(true);
				}
				else
				{
					AEngine::SceneManager::GetActiveScene()->Start();
					AEngine::Application::Instance().GetWindow()->ShowCursor(false);
				}
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
		: Application{ props }
	{
		PushLayer(std::make_unique<DemoLayer>("Test Layer"));
		this->GetWindow()->ShowCursor(false);

		AEngine::RenderCommand::EnableBlend(true);
		AEngine::RenderCommand::SetBlendFunction(AEngine::BlendFunction::SourceAlpha, AEngine::BlendFunction::OneMinusSourceAlpha);
	}
};

AEngine::Application* AEngine::CreateApplication(AEngine::Application::Properties& props)
{
	props.title = "AEngine Demo";
	return new DemoApp(props);
}
