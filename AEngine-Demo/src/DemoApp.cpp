/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine.h>
#include <imgui.h>
#include <cstdlib>
#include <memory>

namespace {
	void FireProjectile(
		AEngine::Math::vec3 startingPosition,
		AEngine::Math::vec3 normalizedDirection)
	{
		using namespace AEngine;
		static int projectileCount = 0;
		constexpr float radius = 0.25f;       // cm
		constexpr float speed = 100.0f;       // m/s
		constexpr float mass = 0.1f;          // kg
		constexpr float restitution = 0.1f;   // perfectly inelastic

		// create the projectile
		Scene* activeScene = SceneManager::GetActiveScene();
		Entity projectile = activeScene->CreateEntity("projectile" + std::to_string(projectileCount));

		// add the transform
		projectile.AddComponent<TransformComponent>(
			startingPosition,
			Math::quat{ Math::vec3{0.0f, 0.0f, 0.0f} },
			Math::vec3{ radius }
		);

		// add the renderable component
		projectile.AddComponent<RenderableComponent>(
			true,
			AssetManager<Model>::Instance().Get("sphere.gltf"),
			AssetManager<Shader>::Instance().Get("simple.shader")
		);

		// add the rigid body component
		RigidBodyComponent *rigidBodyComp = projectile.AddComponent<RigidBodyComponent>();
		rigidBodyComp->ptr = SceneManager::GetActiveScene()->GetPhysicsWorld()->AddRigidBody(
			startingPosition,
			Math::quat{ Math::vec3{0.0f, 0.0f, 0.0f} }
		);

		// set the rigidbody properties
		rigidBodyComp->ptr->SetMass(mass);
		rigidBodyComp->ptr->SetLinearVelocity(normalizedDirection * speed);
		rigidBodyComp->ptr->SetType(RigidBody::Type::Dynamic);
		rigidBodyComp->ptr->SetHasGravity(false);
		rigidBodyComp->ptr->SetRestitution(restitution);

		// add a collider to the rigid body
		rigidBodyComp->ptr->AddSphereCollider(radius);

		// attach script to destroy the projectile
		// ScriptableComponent* scriptComp = projectile.AddComponent<ScriptableComponent>();
		// Script* script = AssetManager<Script>::Instance().Get("projectile.lua").get();
		// scriptComp->script = MakeUnique<EntityScript>(projectile, ScriptEngine::GetState(), script);

		++projectileCount;
	}
}

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
		Scene *physicsScene = SceneManager::LoadFromFile("assets/scenes/physics.scene");
		if (!physicsScene)
		{
			exit(1);
		}

		// set active scene and debug camera
		SceneManager::SetActiveScene("physics");
		Scene::UseDebugCamera(true);
		DebugCamera& debugCam = Scene::GetDebugCamera();
		debugCam.SetFarPlane(10000.0f);
		debugCam.SetMovementSpeed(20.0f);
		debugCam.SetNearPlane(0.1f);
		debugCam.SetFov(45.0f);
		debugCam.SetYaw(-90.0f);

		// setup physics debug renderer
		const PhysicsRenderer* debugRenderer = physicsScene->GetPhysicsRenderer();
		debugRenderer->SetRenderItem(PhysicsRendererItem::CollisionShape, true);
		debugRenderer->SetRenderItem(PhysicsRendererItem::ContactPoint, true);
		debugRenderer->SetRenderShape(CollisionRenderShape::Capsule, true);
		debugRenderer->SetRenderShape(CollisionRenderShape::Box, true);
		debugRenderer->SetRenderShape(CollisionRenderShape::Sphere, true);

		// set scene to simulation mode and turn on physics rendering
		physicsScene->SetState(Scene::State::Simulate);
		physicsScene->SetPhysicsRenderingEnabled(true);

		// add the aabb debug render for physics demo
		debugRenderer->SetRenderItem(PhysicsRendererItem::ColliderAABB, true);
	}

	void OnDetach() override
	{
		// do nothing
	}

	void OnUpdate(AEngine::TimeStep ts) override
	{
		using namespace AEngine;

		if (Input::IsKeyPressedNoRepeat(AEKey::ESCAPE))
		{
			if (Application::Instance().GetWindow()->IsShowingCursor())
			{
				Application::Instance().GetWindow()->ShowCursor(false);
				Application::Instance().EditMode(false);
			}
			else
			{
				Application::Instance().GetWindow()->ShowCursor(true);
				Application::Instance().EditMode(true);
			}
		}

		if (Input::IsMouseButtonPressedNoRepeat(AEMouse::BUTTON_LEFT))
		{
			// get the position and direction of the camera
			// this will be used to orientate the projectile
			Math::vec3 front = Scene::GetDebugCamera().GetFront();
			Math::vec3 pos = Scene::GetDebugCamera().GetPosition();
			FireProjectile(pos, front);
		}

		AEngine::SceneManager::GetActiveScene()->OnUpdate(ts);
	}
};

class DemoApp : public AEngine::Application
{
public:
	DemoApp(AEngine::Application::Properties props)
		: Application{ props }
	{
		SetLayer(std::make_unique<DemoLayer>("Physics Demo"));
		this->GetWindow()->ShowCursor(false);

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
	props.title = "Christien's Physics Demo";
	return new DemoApp(props);
}