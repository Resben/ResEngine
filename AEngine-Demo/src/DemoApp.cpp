/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine.h>
#include <imgui.h>
#include <cstdlib>
#include <memory>

class DemoLayer : public AEngine::Layer
{
public:
	DemoLayer(const std::string& ident)
		: AEngine::Layer(ident)
	{
	}

	void OnAttach() override
	{
		using namespace AEngine;

		// load scenes
		Scene *physicsScene = SceneManager::LoadFromFile("assets/scenes/serialized.scene");
		if (!physicsScene)
		{
			exit(1);
		}

		// set active scene and debug camera
		SceneManager::SetActiveScene("serialized");
		Scene::UseDebugCamera(true);
		DebugCamera& debugCam = Scene::GetDebugCamera();
		debugCam.SetFarPlane(10000.0f);
		debugCam.SetMovementSpeed(20.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);

		// setup physics debug renderer
		PhysicsWorld* physicsWorld = physicsScene->GetPhysicsWorld();
		physicsWorld->SetRenderingEnabled(true);
		const PhysicsRenderer* debugRenderer = physicsWorld->GetRenderer();
		debugRenderer->SetRenderItem(PhysicsRendererItem::CollisionShape, true);
		debugRenderer->SetRenderItem(PhysicsRendererItem::ColliderAABB, true);
		debugRenderer->SetRenderItem(PhysicsRendererItem::ContactPoint, true);
		debugRenderer->SetRenderItem(PhysicsRendererItem::ContactNormal, true);
		debugRenderer->SetRenderShape(CollisionRenderShape::Box, true);
		debugRenderer->SetRenderShape(CollisionRenderShape::Sphere, true);

		// set scene to simulation mode and turn on physics rendering and set a high update rate
		physicsScene->SetState(Scene::State::Edit);
		physicsScene->SetRefreshRate(300);
	}

	void OnDetach() override
	{
		// do nothing
	}

	void OnUpdate(AEngine::TimeStep ts) override
	{
		using namespace AEngine;
		AEngine::SceneManager::GetActiveScene()->OnUpdate(ts);
	}
};

class DemoApp : public AEngine::Application
{
public:
	DemoApp(AEngine::Application::Properties props)
		: Application{ props }
	{
		SetLayer(std::make_unique<DemoLayer>("Demo Layer"));

		// setup render settings
		AEngine::RenderCommand::EnableBlend(true);
		AEngine::RenderCommand::SetBlendFunction(AEngine::BlendFunction::SourceAlpha, AEngine::BlendFunction::OneMinusSourceAlpha);
		AEngine::RenderCommand::EnableFaceCulling(true);
		AEngine::RenderCommand::SetCullFace(AEngine::PolygonFace::Back);
		this->GetWindow()->Maximise();
	}
};

AEngine::Application* AEngine::CreateApplication(AEngine::Application::Properties& props)
{
	props.title = "HAC Interactive Demo";
	return new DemoApp(props);
}
