#pragma once
#include <stack>
#include <string>
#include <EnTT/entt.hpp>
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Physics/Physics.h"
#include "Components.h"
#include "DebugCamera.h"

namespace AEngine
{
	class Entity;

	/**
		 * \class Scene
		 * \brief Scene Class used by the ECS
		 * \author Geoff Candy (34183006)
		 * \author Christien Alden (34119981)
		**/
	class Scene
	{
	public:
//--------------------------------------------------------------------------------
// Initialisation and Management
//--------------------------------------------------------------------------------
			/**
			 * \brief Gets the scene identifier
			 * \return The identifier of the scene
			*/
		const std::string& GetIdent() const;

			/**
			 * \brief Method to create Entities within the Scene
			 * \param[in] name of entity; default is "DefaultEntity"
			 * \return Entity
			**/
		Entity CreateEntity(const std::string& name = std::string());
			/**
			 * \brief Method to retrieve an Entity from the Scene
			 * \param[in] tag of entity
			 * \return Entity
			 * \note Check the Entity::IsValid() method before using the Entity
			*/
		Entity GetEntity(const std::string& tag);
			/**
			 * \brief Method to retrieve an Entity from the Scene
			 * \param[in] ident of entity
			 * \return Entity
			 * \note Check the Entity::IsValid() method before using the Entity
			*/
		Entity GetEntity(Uint16 ident);

//--------------------------------------------------------------------------------
// Events
//--------------------------------------------------------------------------------
			/**
			 * \brief Updates the scene during runtime
			 * \details
			 * This should be called each frame just before the window is refreshed.
			**/
		void OnUpdate(TimeStep dt);

			/**
			 * \brief Updates scene cameras to reflect the new aspect ratio
			 * \param[in] width of scene
			 * \param[in] height of scene
			**/
		void OnViewportResize(unsigned int width, unsigned int height);

//--------------------------------------------------------------------------------
// Simulation
//--------------------------------------------------------------------------------
			/**
			 * \brief Resumes scene simulation
			**/
		void Start();

			/**
			 * \brief Pauses scene simulation
			**/
		void Stop();

			/**
			 * \brief Returns running state of simulation
			 * \retval true if simulation is running
			 * \retval false if simulation is **not** running
			**/
		bool IsRunning();

			/**
			 * \brief Sets the active camera for the scene
			 * \param[in] entityTag of entity with CameraComponent
			 * \retval true if active camera was updated
			 * \retval false if active camera was **not** updated
			 * \details
			 * Searches for an entity with a camera that matches \p entityTag,
			 * the active camera is only changed if a matching entity if found.
			**/
		bool SetActiveCamera(const std::string& entityTag);

//--------------------------------------------------------------------------------
// Debug Camera
//--------------------------------------------------------------------------------
			/**
			 * \brief Sets whether the scene uses the debug camera or an entity camera
			 * \param[in] value true to use debug cam; false to revert to entity cam
			 * \bug Currently controllable entities are still updated
			**/
		static void UseDebugCamera(bool value);

			/**
			 * \brief Returns status of Scene::SetDebugCamera
			 * \retval true if using debug camera
			 * \retval false if **not** using debug camera
			**/
		static bool UsingDebugCamera();

			/**
			 * \brief Returns the debug camera reference to update properties
			 * \return DebugCamera&
			**/
		static DebugCamera& GetDebugCamera();

//--------------------------------------------------------------------------------
	private:
		friend class Entity;
		friend class SceneManagerImpl;
		friend class SceneSerialiser;

		// core
		bool m_isRunning;
		std::string m_ident;
		entt::registry m_Registry;
		PhysicsWorld* m_physicsWorld;

//--------------------------------------------------------------------------------
// Debug Camera
//--------------------------------------------------------------------------------
		static DebugCamera s_debugCamera;
		static bool s_useDebugCamera;

//--------------------------------------------------------------------------------
// Runtime Methods
//--------------------------------------------------------------------------------
			/**
			 * \brief Constructor for Scene
			 * \param[in] ident of scene; default is "Default Scene"
			 * \note There can only be one scene with a given ident
			 * \warning This should not be called directly, use SceneManager::CreateScene() instead
			*/
		Scene(const std::string& ident = "Default Scene");
			/**
			 * \brief Initialises scene
			**/
		void Init(unsigned int updatesPerSecond = 60);
			/**
			 * \brief Calls modern render system with the given camera
			 * \param[in] activeCam to render scene from
			**/
		void RenderOnUpdate(const PerspectiveCamera& activeCam);
			/**
			 * \brief Calls modern terrain system with the given camera
			 * \param[in] activeCam to render scene from
			*/
		void TerrainOnUpdate(const PerspectiveCamera& activeCam);
			/**
			 * \brief Updates cameras in scene
			 * \retval PerspectiveCamera* if there is an active camera
			 * \retval nullptr is there was no active camera
			 * \warning If function returns nullptr, the program will terminate
			 * \note For optimisation reasons, only the active camera will have
			 * its view matrix updated.
			**/
		PerspectiveCamera* CamerasOnUpdate();
			/**
			 * \brief Updates physics in scene
			*/
		void PhysicsOnUpdate();
			/**
			 * \brief Updates scripts in scene
			 * \param[in] dt timestep
			*/
		void ScriptableOnUpdate(TimeStep dt);
	};
}
