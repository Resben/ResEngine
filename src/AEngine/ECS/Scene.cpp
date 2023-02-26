/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Scene and system implementation
**/
#include "AEngine/Core/Logger.h"
#include <cassert>
#include "AEngine/ECS/Scene.h"
#include "AEngine/ECS/Entity.h"
#include "AEngine/ECS/Components.h"
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Render/Renderer.h"
#include "AEngine/Core/Font.h"

namespace AEngine
{
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		auto& tag = entity.AddComponent<TagComponent>();
		if (name.empty())
		{
			tag.tag = "DefaultEntity";
		}
		else
		{
			tag.tag = name;
		}

		// assign id
		tag.id = nextId;
		++nextId;

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

		AE_LOG_ERROR("Entity doesn't exist");
	}

	Entity Scene::GetEntity(uint32_t id)
	{
		auto entityView = m_Registry.view<TagComponent>();
		for (auto [entity, TagComponent] : entityView.each())
		{
			if (TagComponent.id == id)
			{
				return Entity(entity, this);
			}
		}

		AE_LOG_ERROR("Entity doesn't exist");
	}

	//--------------------------------------------------------------------------------
	// Simulation
	//--------------------------------------------------------------------------------

#ifdef AE_DEBUG
	void ShowFPS(TimeStep frametime)
	{
		static int count = 0;
		static float accumulator = 0.0f;

		++count;
		accumulator += frametime.Seconds();
		if (accumulator >= 1.0f)
		{
			AE_LOG_DEBUG("fps [{}]", count);
			count = 0;
			accumulator -= 1.0f;
		}
	}
#endif

	void Scene::OnUpdate()
	{
		static bool first = true;
		if (first)
		{
			sceneClock.Reset();
			first = false;
		}

		TimeStep frameTime = sceneClock.Update();

#ifdef AE_DEBUG
		ShowFPS(frameTime);
#endif

		// update player score
		Entity player = GetEntity("Player");
		FontComponent& font = player.GetComponent<FontComponent>();
		ScoreComponent& score = player.GetComponent<ScoreComponent>();
		font.textToRender = std::to_string(score.score);
		score.timeRemaining = score.timeRemaining - frameTime.Seconds();
		if (score.timeRemaining <= 0.0f)
			score.timeRemaining = 0.0f;

		// update timer
		Entity timer = GetEntity("Timer");
		FontComponent& timeFont = timer.GetComponent<FontComponent>();
		timeFont.textToRender = std::to_string(score.timeRemaining);

		if (IsRunning())
		{
			UpdateCooldown(frameTime);
			PhysicsOnUpdate(frameTime);
		}

		PerspectiveCamera* activeCam = nullptr;
		if (m_useDebugCamera)
		{
			m_debugCam.OnUpdate(frameTime);
			activeCam = &m_debugCam;
		}
		else
		{
			activeCam = CamerasOnUpdate();
		}

		assert(activeCam != nullptr);
		RenderOnUpdate(*activeCam);
	}

	void Scene::Init(unsigned int updatesPerSecond)
	{
		assert(updatesPerSecond != 0);
		m_physicsStep = TimeStep{ 1.0f / updatesPerSecond };
		m_physicsWorld.Init({m_physicsStep});
	}

	void Scene::Pause()
	{
		sceneClock.Pause();
	}

	void Scene::Resume()
	{
		sceneClock.Resume();
	}

	bool Scene::IsRunning()
	{
		return sceneClock.IsRunning();
	}

	Physics& Scene::GetPhysicsWorld()
	{
		return m_physicsWorld;
	}

	void Scene::OnViewportResize(unsigned int width, unsigned int height)
	{
		// avoid divide by zero error
		if (height == 0) { height = 1; }

			// Remove these :(
		m_width = (float)width;
		m_height = (float)height;

		// update each camera's aspect ratio
		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto [entity, cameraComp] : cameraView.each())
		{
			cameraComp.cam.SetAspect((float) width / height);
		}
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

	void Scene::UpdateCooldown(float frametime)
	{
		auto cooldownView = m_Registry.view<CooldownComponent>();
		for (auto [entity, cooldown] : cooldownView.each())
		{
			if (!cooldown.active()) // add to cooldown time
			{
				cooldown.time += cooldown.add;
			}
			else // decrement cooldown time
			{
				cooldown.time -= frametime;
			}

			// remove any pending add requests
			cooldown.add = 0.0f;
		}

		// apply to renderable
		auto cooldownRenderable = m_Registry.view<CooldownComponent, RenderableComponent>();
		for (auto [entity, cooldown, renderComp] : cooldownRenderable.each())
		{
			renderComp.active = !cooldown.active();
		}
	}

	//--------------------------------------------------------------------------------
	// Debugging
	//--------------------------------------------------------------------------------
	void Scene::SetDebugCamera(bool value)
	{
		m_useDebugCamera = value;
	}

	bool Scene::IsUsingDebugCamera() const
	{
		return m_useDebugCamera;
	}

	DebugCamera& Scene::GetDebugCamera()
	{
		return m_debugCam;
	}

	//--------------------------------------------------------------------------------
	// Modern Systems
	//--------------------------------------------------------------------------------
	void Scene::ControllableOnUpdate()
	{
		auto controlView = m_Registry.view<ControllableComponent, PhysicsComponent>();
		for (auto [entity, controlComp, physicsComp] : controlView.each())
		{
			if (controlComp.callback)
				controlComp.callback(physicsComp.body);
		}
	}

	void Scene::PhysicsOnUpdate(TimeStep frameTime)
	{
		static float accumulator{ 0.0f };
		accumulator += frameTime;
		if (accumulator >= m_physicsStep)
		{
			ControllableOnUpdate();
			accumulator -= m_physicsStep;
		}

		m_physicsWorld.Update(frameTime.Seconds());

		auto physicsView = m_Registry.view<PhysicsComponent, TransformComponent>();
		for (auto [entity, physicsComp, transformComp] : physicsView.each())
		{
		 	physicsComp.body.GetTransform(
			transformComp.translation,
		 	transformComp.rotation
		 	);
		 }
	}

	PerspectiveCamera* Scene::CamerasOnUpdate()
	{
		PerspectiveCamera* activeCam{ nullptr };
		auto cameraView = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto [entity, cameraComp, transformComp] : cameraView.each())
		{
			if (cameraComp.active)
			{
				cameraComp.cam.SetViewMatrix(glm::inverse(transformComp.ToMat4()));
				activeCam = &cameraComp.cam;
				break;
			}
		}

		return activeCam;
	}

	void Scene::RenderOnUpdate(const PerspectiveCamera& activeCam)
	{
		Renderer* renderer = Renderer::Instance();

		auto lightView = m_Registry.view<TransformComponent, LightComponent>();
		for (auto [entity, transformComp, lightComp] : lightView.each())
		{
			renderer->SetProjection(
				activeCam.GetProjectionViewMatrix(),
				{transformComp.translation, lightComp.colour}
			);
		}

		auto renderView = m_Registry.view<RenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				renderer->Submit(
					*renderComp.mesh, *renderComp.texture,
					*renderComp.shader, transformComp.ToMat4()
				);
			}
		}

		auto fontView = m_Registry.view<FontComponent>();
		for (auto [entity, fontComp] : fontView.each())
		{
			fontComp.font->RenderText(
				*fontComp.shader,
				fontComp.textToRender,
				fontComp.pos,
				fontComp.scale,
				fontComp.colour,
				m_width,
				m_height
			);
		}
	}
}
