/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Scene and system implementation
**/
#include "../Core/Logger.h"
#include <cassert>
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "../Core/PerspectiveCamera.h"
#include "../Render/Renderer.h"

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

		if (IsRunning())
		{
			//stubs 
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
		//stubs used for physics world initialisation
	}

	void Scene::Pause()
	{
		sceneClock.Stop();
	}

	void Scene::Resume()
	{
		sceneClock.Start();
	}

	bool Scene::IsRunning()
	{
		return sceneClock.IsRunning();
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

	PerspectiveCamera* Scene::CamerasOnUpdate()
	{
		PerspectiveCamera* activeCam{ nullptr };
		auto cameraView = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto [entity, cameraComp, transformComp] : cameraView.each())
		{
			if (cameraComp.active)
			{
				cameraComp.cam.SetViewMatrix(Math::inverse(transformComp.ToMat4()));
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
			//renderer->SetProjection(
			//	activeCam.GetProjectionViewMatrix(),
			//	{transformComp.translation, lightComp.colour}
			//);
		}

		auto renderView = m_Registry.view<RenderableComponent, TransformComponent>();
		for (auto [entity, renderComp, transformComp] : renderView.each())
		{
			if (renderComp.active)
			{
				//renderer->Submit(
				//	*renderComp.mesh, *renderComp.texture,
				//	*renderComp.shader, transformComp.ToMat4()
				//);
			}
		}
	}
}
