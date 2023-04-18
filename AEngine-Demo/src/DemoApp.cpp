/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine.h>

using namespace AEngine;
class DemoLayer : public AEngine::Layer
{
public:
	void onAttach() override
	{
		m_scene = std::make_shared<AEngine::Scene>("Test Scene");
		m_scene->LoadFromFile("assets/scenes/test.scene");
		// m_scene->LoadFromFile("assets/scenes/export.scene");

		m_scene->UseDebugCamera(true);
		AEngine::DebugCamera& debugCam = m_scene->GetDebugCamera();
		debugCam.SetFarPlane(100.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);

		m_scene->Init();
		m_scene->Start();

		Entity box = m_scene->GetEntity("Box1");
		ScriptableComponent* script = box.AddComponent<ScriptableComponent>("test.lua");
	}

	void onDetach() override
	{
		m_scene->SaveToFile("assets/scenes/export.scene");
	}

	void onUpdate(AEngine::TimeStep ts) override
	{
		AEngine::EventDispatcher e;
		// capture keyevent for testing
		e.Dispatch<KeyPressed>([&, this](KeyPressed& e) -> bool {
			switch (e.GetKey())
			{
			case AEKey::ESCAPE:
				Application::Instance().Terminate();
				break;
			case AEKey::F1:
				m_scene->TakeSnapshot();
				break;
			case AEKey::F2:
				m_scene->RestoreSnapshot();
				break;
			case AEKey::C:
				Entity entity = m_scene->GetEntity("Box1");
				TransformComponent* trans = entity.GetComponent<TransformComponent>();
				trans->scale *= Math::vec3(1.5, 1.5, 1.5);
				break;
			}
			return true;
			});

		m_scene->OnUpdate(ts);
	}

private:
	std::shared_ptr<Scene> m_scene;
};

class DemoApp : public Application
{
public:
	DemoApp(ApplicationProps props)
		: Application(props)
	{
		SetLayer(new DemoLayer());
	}
};

AEngine::Application* AEngine::CreateApplication()
{
	AEngine::ApplicationProps props;
	props.title = "AEngine Demo from outside engine";

	return new DemoApp(props);
}
