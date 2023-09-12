/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Scene and system implementation
**/
#include "Scene.h"
#include "AEngine/Core/Identifier.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Messaging/MessageService.h"
#include "AEngine/Physics/PlayerController.h"
#include "AEngine/Skybox/Skybox.h"
#include "AEngine/Water/Water.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Window.h"
#include "AEngine/Render/RenderCommand.h"
#include "AEngine/Render/RenderPipeline.h"
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
		: m_ident(ident), m_fixedTimeStep{ 1.0f / 60.0f }
	{
		RenderPipeline::Instance().SetTargets({RenderPipelineTarget::Positon, RenderPipelineTarget::Normal, RenderPipelineTarget::Diffuse });
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

	void Scene::GetEntityIds(std::vector<Uint16> &entityids)
	{
		auto entityView = m_Registry.view<TagComponent>();
		for(auto [entity, tc] : entityView.each())
		{
			entityids.push_back(tc.ident);
		}
	}

	void Scene::PurgeEntitiesStagedForRemoval()
	{
		auto it = m_entitiesStagedForRemoval.begin();
		while (it != m_entitiesStagedForRemoval.end())
		{
			m_Registry.destroy(*it);
			it = m_entitiesStagedForRemoval.erase(it);
		}
	}

//--------------------------------------------------------------------------------
// Events
//--------------------------------------------------------------------------------
	void Scene::Init()
	{
		m_isRunning = false;
		m_physicsWorld = PhysicsAPI::Instance().CreateWorld({ m_fixedTimeStep });

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
		TimeStep adjustedDt = dt.Seconds(IsRunning() ? 1.0f : 0.0f);

		// update simulation
		MessageService::DispatchMessages();
		ScriptOnUpdate(adjustedDt);
		ScriptOnFixedUpdate(adjustedDt);
		PhysicsOnUpdate(adjustedDt);
		ScriptOnLateUpdate(adjustedDt);

		// purge entities that have been marked for deletion
		PurgeEntitiesStagedForRemoval();

		// render simulation
		PerspectiveCamera* activeCam = m_activeCamera;
		if (s_useDebugCamera)
		{
			if (m_isRunning || m_edit)
			{
				s_debugCamera.OnUpdate(dt);
			}
			activeCam = &s_debugCamera;
		}

		CameraOnUpdate();

		RenderPipeline::Instance().BindGeometryPass();
		RenderOpaqueOnUpdate(activeCam);
		AnimateOnUpdate(activeCam, adjustedDt);
		RenderPipeline::Instance().UnbindGeometryPass();
		RenderPipeline::Instance().LightingPass();
		SkyboxOnUpdate(activeCam);
		RenderTransparentOnUpdate(activeCam);

		if (m_physicsWorld->IsRenderingEnabled())
		{
			m_physicsWorld->Render(activeCam->GetProjectionViewMatrix());
		}
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

	void Scene::Edit(bool toggle)
	{
		m_edit = toggle;
	}

	bool Scene::IsEditing() const
	{
		return m_edit;
	}

//--------------------------------------------------------------------------------
// Runtime Methods
//--------------------------------------------------------------------------------
	void Scene::PhysicsOnUpdate(TimeStep dt)
	{
		// update physics simulation
		m_physicsWorld->OnUpdate(dt);

		// get transforms for physics handles
		auto physicsView = m_Registry.view<PhysicsHandle, TransformComponent>();
		for (auto [entity, ph, tc] : physicsView.each())
		{
			if (ph.ptr)
			{
				ph.ptr->GetTransform(tc.translation, tc.orientation);
			}
		}

		// get transforms for player controllers
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

	void Scene::ScriptOnUpdate(TimeStep dt)
	{
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto [entity, script] : scriptView.each())
		{
			script.script->OnUpdate(dt);
		}
	}

	void Scene::ScriptOnFixedUpdate(TimeStep dt)
	{
		static TimeStep accumulator{ 0.0f };
		accumulator += dt;
		if (accumulator < m_fixedTimeStep)
		{
			return;
		}

		accumulator -= m_fixedTimeStep;
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto [entity, script] : scriptView.each())
		{
			script.script->OnFixedUpdate(m_fixedTimeStep);
		}
	}

	void Scene::ScriptOnLateUpdate(TimeStep dt)
	{
		auto scriptView = m_Registry.view<ScriptableComponent>();
		for (auto [entity, script] : scriptView.each())
		{
			script.script->OnLateUpdate(dt);
		}
	}

	void Scene::RenderOpaqueOnUpdate(const PerspectiveCamera* activeCam)
	{
		if (activeCam == nullptr)
		{
			return;
		}

		auto renderView = m_Registry.view<RenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				renderComp.model->RenderOpaque(
					transformComp.ToMat4(), *renderComp.shader, activeCam->GetProjectionViewMatrix()
				);
			}
		}
	}

	void Scene::RenderTransparentOnUpdate(const PerspectiveCamera* activeCam)
	{
		if (activeCam == nullptr)
		{
			return;
		}

		auto renderView = m_Registry.view<RenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				renderComp.model->RenderTransparent(
					transformComp.ToMat4(), *RenderPipeline::Instance().GetTransparentShader(), activeCam->GetProjectionViewMatrix()
				);
			}
		}
	}

	void Scene::AnimateOnUpdate(const PerspectiveCamera* activeCam, const TimeStep dt)
	{
		if (activeCam == nullptr)
		{
			return;
		}

		auto renderView = m_Registry.view<SkinnedRenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				renderComp.model->Render(
					transformComp.ToMat4(),
					*renderComp.shader,
					activeCam->GetProjectionViewMatrix(),
					renderComp.animator,
					dt
				);
			}
		}
	}

	void Scene::SkyboxOnUpdate(const PerspectiveCamera* camera)
	{
		if (camera == nullptr)
		{
			return;
		}

		auto skyboxView = m_Registry.view<SkyboxComponent>();
		for (auto [entity, skyboxComp] : skyboxView.each())
		{
			if (skyboxComp.active)
			{
				skyboxComp.skybox->Render(*(skyboxComp.shader), camera->GetProjectionMatrix(), camera->GetViewMatrix());
			}
		}
	}

//--------------------------------------------------------------------------------
// PhysicsRenderer
//--------------------------------------------------------------------------------
		void Scene::SetPhysicsRenderingEnabled(bool enable) const
		{
			m_physicsWorld->SetRenderingEnabled(enable);
		}

		bool Scene::IsPhysicsRenderingEnabled() const
		{
			return m_physicsWorld->IsRenderingEnabled();
		}

		const PhysicsRenderer* Scene::GetPhysicsRenderer() const
		{
			return m_physicsWorld->GetRenderer();
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
