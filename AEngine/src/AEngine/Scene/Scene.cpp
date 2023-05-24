/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Scene and system implementation
**/
#include "Scene.h"
#include "AEngine/Core/Identifier.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Physics/PlayerController.h"
#include "AEngine/Render/Renderer.h"
#include "Components.h"
#include "Entity.h"
#include "SceneSerialiser.h"
#include <cassert>
#include <fstream>

namespace AEngine
{
//--------------------------------------------------------------------------------
// Static Initialisation
//--------------------------------------------------------------------------------
	DebugCamera Scene::s_debugCamera = DebugCamera();
	bool Scene::s_useDebugCamera = false;

//--------------------------------------------------------------------------------
// Initialisation and Management
//--------------------------------------------------------------------------------
	const std::string & Scene::GetIdent() const
	{
		return m_ident;
	}

	Scene::Scene(const std::string& ident)
		: m_ident(ident)
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		TagComponent* tag = entity.AddComponent<TagComponent>();
		if (name.empty())
		{
			tag->tag = "DefaultEntity";
		}
		else
		{
			tag->tag = name;
		}

		tag->ident = Identifier::Generate();
		return entity;
	}

	Entity Scene::GetEntity(const std::string& tag)
	{
		auto entityView = m_Registry.view<TagComponent>();
		for (auto [entity, TagComponent] : entityView.each())
		{
			if (TagComponent.tag == tag)
			{
				return Entity(entity, this);
			}
		}

		return Entity(entt::null, this);
	}

	Entity Scene::GetEntity(Uint16 ident)
	{
		auto entityView = m_Registry.view<TagComponent>();
		for (auto [entity, TagComponent] : entityView.each())
		{
			if (TagComponent.ident == ident)
			{
				return Entity(entity, this);
			}
		}

		return Entity(entt::null, this);
	}

	void Scene::RemoveEntity(const std::string &tag)
	{
		Entity entt = GetEntity(tag);
		if (entt.IsValid())
		{
			entt.Destroy();
		}
	}

	void Scene::RemoveEntity(Uint16 ident)
	{
		Entity entt = GetEntity(ident);
		if (entt.IsValid())
		{
			entt.Destroy();
		}
	}

//--------------------------------------------------------------------------------
// Events
//--------------------------------------------------------------------------------
	void Scene::Init(unsigned int updatesPerSecond)
	{
		if (updatesPerSecond == 0)
		{
			AE_LOG_FATAL("Scene::Init::Failed -> updatesPerSecond must not be zero");
		}

		m_isRunning = false;
		m_physicsWorld = PhysicsAPI::Instance().CreateWorld({ 1.0f / updatesPerSecond });

		/// \todo Move this to a better place!
		auto rigidBodyView = m_Registry.view<RigidBodyComponent, TransformComponent>();
		for (auto [entity, rbc, tc] : rigidBodyView.each())
		{
			rbc.ptr = m_physicsWorld->AddRigidBody(tc.translation, tc.orientation);
			rbc.ptr->SetHasGravity(rbc.hasGravity);
			rbc.ptr->SetMass(rbc.massKg);
			rbc.ptr->SetType(rbc.type);

			PhysicsHandle& handle = m_Registry.emplace<PhysicsHandle>(entity);
			handle.ptr = dynamic_cast<CollisionBody*>(rbc.ptr);
		}

		auto boxColliderView = m_Registry.view<BoxColliderComponent, TransformComponent>();
		for (auto [entity, bcc, tc] : boxColliderView.each())
		{
			if (m_Registry.all_of<PhysicsHandle>(entity))
			{
				PhysicsHandle& handle = m_Registry.get<PhysicsHandle>(entity);
				bcc.ptr = handle.ptr->AddBoxCollider(bcc.size);
				bcc.ptr->SetIsTrigger(bcc.isTrigger);
			}
			else
			{
				PhysicsHandle& handle = m_Registry.emplace<PhysicsHandle>(entity);
				handle.ptr = m_physicsWorld->AddCollisionBody(tc.translation, tc.orientation);
				bcc.ptr = handle.ptr->AddBoxCollider(bcc.size);
				bcc.ptr->SetIsTrigger(bcc.isTrigger);
			}
		}

		auto playerControllerView = m_Registry.view<PlayerControllerComponent, TransformComponent>();
		for (auto [entity, pcc, tc] : playerControllerView.each())
		{
			pcc.ptr = new PlayerController(
				m_physicsWorld,
				tc.translation,
				{ pcc.radius, pcc.height, pcc.speed, pcc.moveDrag, pcc.fallDrag }
			);
		}
	}

	void Scene::OnUpdate(TimeStep dt)
	{
		// update simulation
		if (IsRunning())
		{
			m_physicsWorld->OnUpdate(dt);
			PhysicsOnUpdate(dt);
			ScriptableOnUpdate(dt);
		}

		// render simulation
		PerspectiveCamera* activeCam = m_activeCamera;
		if (s_useDebugCamera)
		{
			s_debugCamera.OnUpdate(dt);
			activeCam = &s_debugCamera;
		}

		CameraOnUpdate();
		RenderOnUpdate(activeCam);
		TerrainOnUpdate(activeCam);
	}

	void Scene::OnViewportResize(unsigned int width, unsigned int height)
	{
		// avoid divide by zero error
		if (height == 0) { height = 1; }

		// update each camera's aspect ratio
		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto [entity, cameraComp] : cameraView.each())
		{
			cameraComp.camera.SetAspect((float) width / height);
		}
	}

//--------------------------------------------------------------------------------
// Simulation
//--------------------------------------------------------------------------------
	void Scene::Start()
	{
		AE_LOG_DEBUG("Scene::Start");
		m_isRunning = true;
	}

	void Scene::Stop()
	{
		AE_LOG_DEBUG("Scene::Stop");
		m_isRunning = false;
	}

	bool Scene::IsRunning()
	{
		return m_isRunning;
	}

	void Scene::SetActiveCamera(PerspectiveCamera* camera)
	{
		m_activeCamera = camera;
	}

//--------------------------------------------------------------------------------
// Runtime Methods
//--------------------------------------------------------------------------------
	void Scene::PhysicsOnUpdate(TimeStep dt)
	{
		auto physicsView = m_Registry.view<PhysicsHandle, TransformComponent>();
		for (auto [entity, ph, tc] : physicsView.each())
		{
			if (ph.ptr)
			{
				ph.ptr->GetTransform(tc.translation, tc.orientation);
			}
		}

		auto playerControllerView = m_Registry.view<PlayerControllerComponent, TransformComponent>();
		for (auto [entity, pcc, tc] : playerControllerView.each())
		{
			if (pcc.ptr)
			{
				pcc.ptr->OnUpdate(dt);
				tc.translation = pcc.ptr->GetTransform();
			}
		}
	}

	void Scene::CameraOnUpdate()
	{
		auto cameraView = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto [entity, cameraComp, transformComp] : cameraView.each())
		{
			cameraComp.camera.SetViewMatrix(Math::inverse(transformComp.ToMat4()));
		}
	}

	void Scene::ScriptableOnUpdate(TimeStep dt)
	{
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto [entity, script] : scriptView.each())
		{
			script.script->OnUpdate(dt);
		}
	}

	void Scene::RenderOnUpdate(const PerspectiveCamera* activeCam)
	{
		if (activeCam == nullptr)
		{
			return;
		}

		Renderer* renderer = Renderer::Instance();

		// set the new projection view matrix
		renderer->SetProjection(activeCam->GetProjectionViewMatrix());
		auto renderView = m_Registry.view<RenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				renderer->Submit(
					*renderComp.model,*renderComp.shader, transformComp.ToMat4()
				);
			}
		}
	}

	void Scene::TerrainOnUpdate(const PerspectiveCamera* camera)
	{
		if (camera == nullptr)
		{
			return;
		}

		Renderer* renderer = Renderer::Instance();

		// set the new projection view matrix
		renderer->SetProjection(camera->GetProjectionViewMatrix());

		auto renderView = m_Registry.view<TerrainComponent, TransformComponent>();
		for (auto [entity, terrainComp, transformComp] : renderView.each())
		{
			if (terrainComp.active)
			{
				renderer->SubmitTerrain(
					terrainComp.textures, terrainComp.yRange, *terrainComp.terrain, *terrainComp.shader, transformComp.ToMat4()
				);
			}
		}
	}

//--------------------------------------------------------------------------------
// Debug Camera
//--------------------------------------------------------------------------------
	void Scene::UseDebugCamera(bool value)
	{
		s_useDebugCamera = value;
	}

	bool Scene::UsingDebugCamera()
	{
		return s_useDebugCamera;
	}

	DebugCamera& Scene::GetDebugCamera()
	{
		return s_debugCamera;
	}
}
