/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine.h>

#include <iostream>
using namespace AEngine;

void playerController(Entity entity, TimeStep dt)
{
	TransformComponent* trans = entity.GetComponent<TransformComponent>();
	InputQuery& in = Application::Instance().Input();
	float offset = 10.0f * dt;

	// forward/back
	if (in.IsKeyPressed(AEKey::W))
		trans->translation -= Math::normalize(Math::rotateVec(Math::vec3(0.0f, 0.0f, 1.0f), trans->rotation)) * offset;
	if (in.IsKeyPressed(AEKey::S))
		trans->translation += Math::normalize(Math::rotateVec(Math::vec3(0.0f, 0.0f, 1.0f), trans->rotation)) * offset;

	// strafe
	if (in.IsKeyPressed(AEKey::A))
		trans->translation -= Math::normalize(Math::rotateVec(Math::vec3(1.0f, 0.0f, 0.0f), trans->rotation)) * offset;
	if (in.IsKeyPressed(AEKey::D))
		trans->translation += Math::normalize(Math::rotateVec(Math::vec3(1.0f, 0.0f, 0.0f), trans->rotation)) * offset;

	// up/down
	if (in.IsKeyPressed(AEKey::SPACE))
		trans->translation += Math::normalize(Math::rotateVec(Math::vec3(0.0f, 1.0f, 0.0f), trans->rotation)) * offset;
	if (in.IsKeyPressed(AEKey::LEFT_SHIFT))
		trans->translation -= Math::normalize(Math::rotateVec(Math::vec3(0.0f, 1.0f, 0.0f), trans->rotation)) * offset;

	// look
	if (in.IsKeyPressed(AEKey::UP))
		trans->rotation = Math::rotate(trans->rotation, Math::radians(1.0f) * offset * 5.0f, Math::normalize(Math::vec3(-90.0, 0.0, 0.0)));
	if (in.IsKeyPressed(AEKey::DOWN))
		trans->rotation = Math::rotate(trans->rotation, Math::radians(1.0f) * offset * 5.0f, Math::normalize(Math::vec3(90.0, 0.0, 0.0)));

	if (in.IsKeyPressed(AEKey::LEFT))
		trans->rotation = Math::rotate(trans->rotation, Math::radians(1.0f) * offset * 5.0f, Math::normalize(Math::vec3(0.0, 90.0f, 0.0)));
	if (in.IsKeyPressed(AEKey::RIGHT))
		trans->rotation = Math::rotate(trans->rotation, Math::radians(1.0f) * offset * 5.0f, Math::normalize(Math::vec3(0.0, -90.0f, 0.0)));
}

class DemoLayer : public AEngine::Layer
{
public:
	void OnAttach() override
	{
		m_scene = std::make_shared<AEngine::Scene>("Test Layer");
		m_scene->LoadFromFile("assets/scenes/test.scene");
		 //m_scene->LoadFromFile("assets/scenes/export.scene");

		m_scene->UseDebugCamera(false);
		AEngine::DebugCamera& debugCam = m_scene->GetDebugCamera();
		debugCam.SetFarPlane(100.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);

		m_scene->Init();
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
			case AEKey::F3:
			{
				m_scene->UseDebugCamera(!m_scene->UsingDebugCamera());
				//m_scene->GetEntity("Enemy").GetComponent<RenderableComponent>()->active = !m_scene->GetEntity("Enemy").GetComponent<RenderableComponent>()->active;
				break;
			}
			case AEKey::F5:
				m_scene->LoadFromFile("assets/scenes/test.scene");
				break;
	/*		case AEKey::C:
				Entity entity = m_scene->GetEntity("Box1");
				TransformComponent* trans = entity.GetComponent<TransformComponent>();
				trans->rotation = Math::rotate(trans->rotation, Math::radians(45.0f), Math::normalize(Math::vec3(0, 90.0, 0.0)));
				break;*/
			}
			return true;
			});

		if (!m_scene->UsingDebugCamera())
		{
			Entity entity = m_scene->GetEntity("Enemy");
			playerController(entity, ts);
		}

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
		PushLayer(new DemoLayer());
	}
};

AEngine::Application* AEngine::CreateApplication()
{
	AEngine::ApplicationProps props;
	props.title = "AEngine Demo from outside engine";

	return new DemoApp(props);
}
