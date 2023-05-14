/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Scene and system implementation
**/
#include <cassert>
#include <fstream>
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Render/Renderer.h"
#include "Components.h"
#include "Entity.h"
#include "Scene.h"
#include "SceneSerialiser.h"

namespace AEngine
{
	Scene::Scene(const std::string& ident)
		: m_ident(ident), m_debugCam()
	{
		Init();
	}

	Entity Scene::CreateEntity(Uint16 ident, const std::string& name)
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

		tag->ident = ident;
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

//--------------------------------------------------------------------------------
// Snapshots
//--------------------------------------------------------------------------------
	void Scene::LoadFromFile(const std::string& fname)
	{
		SceneSerialiser::DeserialiseFile(this, fname);
		TakeSnapshot();

		/// @todo move this out of here!!!
		auto rigidBodyView = m_Registry.view<RigidBodyComponent, TransformComponent>();
		for (auto [entity, rbc, tc] : rigidBodyView.each())
		{
			rbc.ptr = m_physicsWorld->AddRigidBody(tc.translation, tc.rotation);
			rbc.ptr->SetHasGravity(rbc.hasGravity);
			rbc.ptr->SetMass(rbc.massKg);

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
				handle.ptr = m_physicsWorld->AddCollisionBody(tc.translation, tc.rotation);
				bcc.ptr = handle.ptr->AddBoxCollider(bcc.size);
				bcc.ptr->SetIsTrigger(bcc.isTrigger);
			}
		}
	}

	void Scene::SaveToFile(const std::string& fname)
	{
		SceneSerialiser::SerialiseFile(this, fname);
	}

	void Scene::TakeSnapshot()
	{
		AE_LOG_DEBUG("Taking snapshot");
		m_snapshots.push(Memento(SceneSerialiser::SerialiseNode(this), m_isRunning));
	}

	void Scene::RestoreSnapshot()
	{
		if (m_snapshots.empty())
		{
			return;
		}

		AE_LOG_DEBUG("Restoring snapshot");
		Memento& memento = m_snapshots.top();
		this->m_isRunning = memento.GetIsRunning();
		SceneSerialiser::DeserialiseNode(this, memento.GetRegistry());

		// don't pop the last snapshot off the list
		if (m_snapshots.size() != 1)
		{
			m_snapshots.pop();
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
	}

	void Scene::OnUpdate(TimeStep dt)
	{
		if (IsRunning())
		{
			m_physicsWorld->OnUpdate(dt);
			PhysicsOnUpdate();
			ScriptableOnUpdate(dt);
		}

		PerspectiveCamera* activeCam = nullptr;
		if (m_useDebugCamera)
		{
			m_debugCam.OnUpdate(dt);
			activeCam = &m_debugCam;
		}
		else
		{
			activeCam = CamerasOnUpdate();
		}

		assert(activeCam != nullptr);
		RenderOnUpdate(*activeCam);
		TerrainOnUpdate(*activeCam);
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

	bool Scene::SetActiveCamera(const std::string& entityTag)
	{
		// only set one camera as active camera
		CameraComponent* lastActive{ nullptr };
		bool found{ false };

		auto cameraEntityView = m_Registry.view<CameraComponent, TagComponent>();
		for (auto [entity, cameraComp, tagComp] : cameraEntityView.each())
		{
			// save old active camera
			if (cameraComp.active)
			{
				lastActive = &cameraComp;
			}

			// update if found
			if (entityTag == tagComp.tag)
			{
				cameraComp.active = true;
				found = true;
			}
			else // set all others to false
			{
				cameraComp.active = false;
			}
		}

		// reset last active if new active not found
		if (!found && lastActive)
		{
			lastActive->active = true;
		}

		return found;
	}

//--------------------------------------------------------------------------------
// Debug Camera
//--------------------------------------------------------------------------------
	void Scene::UseDebugCamera(bool value)
	{
		m_useDebugCamera = value;
	}

	bool Scene::UsingDebugCamera() const
	{
		return m_useDebugCamera;
	}

	DebugCamera& Scene::GetDebugCamera()
	{
		return m_debugCam;
	}

	void Scene::ScriptableOnUpdate(TimeStep dt)
	{
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto [entity, script] : scriptView.each())
		{
			script.script->OnUpdate(dt, Entity(entity,this));
		}
	}

//--------------------------------------------------------------------------------
// Systems
//--------------------------------------------------------------------------------
	PerspectiveCamera* Scene::CamerasOnUpdate()
	{
		PerspectiveCamera* activeCam{ nullptr };
		auto cameraView = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto [entity, cameraComp, transformComp] : cameraView.each())
		{
			if (cameraComp.active)
			{
				cameraComp.camera.SetViewMatrix(Math::inverse(transformComp.ToMat4()));
				activeCam = &cameraComp.camera;
				break;
			}
		}

		return activeCam;
	}


	void Scene::PhysicsOnUpdate()
	{
		auto physicsView = m_Registry.view<PhysicsHandle, TransformComponent>();
		for (auto [entity, ph, tc] : physicsView.each())
		{
			if (ph.ptr)
			{
				ph.ptr->GetTransform(tc.translation, tc.rotation);
			}
		}
	}

	void Scene::RenderOnUpdate(const PerspectiveCamera& activeCam)
	{
		Renderer* renderer = Renderer::Instance();

		// set the new projection view matrix
		renderer->SetProjection(activeCam.GetProjectionViewMatrix());

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

	void Scene::TerrainOnUpdate(const PerspectiveCamera& activeCam)
	{
		Renderer* renderer = Renderer::Instance();

		// set the new projection view matrix
		renderer->SetProjection(activeCam.GetProjectionViewMatrix());

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
// Memento
//--------------------------------------------------------------------------------
	Scene::Memento::Memento(YAML::Node registry, bool isRunning)
		: m_registry(registry), m_isRunning(isRunning)
	{

	}

	YAML::Node Scene::Memento::GetRegistry() const
	{
		return m_registry;
	}

	bool Scene::Memento::GetIsRunning() const
	{
		return m_isRunning;
	}
}
