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

	}

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

//--------------------------------------------------------------------------------
// Snapshots
//--------------------------------------------------------------------------------
	void Scene::LoadFromFile(const std::string& fname)
	{
		SceneSerialiser::DeserialiseFile(this, fname);
	}

	void Scene::SaveToFile(const std::string& fname)
	{
		AE_LOG_ERROR("Scene::SaveToFile::Error -> Not implemented");
		SceneSerialiser::SerialiseFile(this, fname);
	}

	//Memento Scene::TakeSnapshot()
	//{
	//	return Serialiser::Serialise(this);
	//}

	//void Scene::RestoreSnapshot(Memento memento)
	//{
	//	this->m_isRunning = memento.GetIsRunning();
	//	Serialiser::Deserialise(this, memento.GetRegistry());
	//}

//--------------------------------------------------------------------------------
// Events
//--------------------------------------------------------------------------------
	void Scene::Init(unsigned int updatesPerSecond)
	{
		assert(updatesPerSecond != 0);
		//stubs used for physics world initialisation

		Start();
	}

	void Scene::OnUpdate(TimeStep dt)
	{
		if (IsRunning())
		{
			//stubs
			// use dt
			auto transformView = m_Registry.view<TransformComponent>();
			for (auto [entity, transformComp] : transformView.each())
			{
				//AE_LOG_DEBUG("Transform -> {} , {} , {}", transformComp.translation.x, transformComp.translation.y, transformComp.translation.z);
			}
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
		m_isRunning = true;
	}

	void Scene::Stop()
	{
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
