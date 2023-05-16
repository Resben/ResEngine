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
//--------------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------------
	class Entity;

	/**
		 * @class Scene
		 * @brief Scene Class used by the ECS
		 * @author Geoff Candy (34183006)
		 * @author Christien Alden (34119981)
		**/
	class Scene
	{
	public:

		const std::string& GetIdent() const;

//--------------------------------------------------------------------------------
// Memento
//--------------------------------------------------------------------------------
		class Memento
		{
		public:
			Memento() = default;
			Memento(YAML::Node registry, bool isRunning);
			YAML::Node GetRegistry() const;
			bool GetIsRunning() const;
			operator bool() { return !m_registry.IsNull(); }

		private:
			YAML::Node m_registry;
			bool m_isRunning;
		};


		Scene(const std::string& ident = "Default Scene");
			/**
			 * @brief Method to create Entities within the Scene
			 * @param[in] name of entity; default is "DefaultEntity"
			 * @return Entity
			**/
		Entity CreateEntity(const std::string& name = std::string());
		Entity GetEntity(const std::string& tag);
		Entity GetEntity(Uint16 ident);

//--------------------------------------------------------------------------------
// Scene Management
//--------------------------------------------------------------------------------
		void LoadFromFile(const std::string& fname);
		void SaveToFile(const std::string& fname);

		void TakeSnapshot();
		void RestoreSnapshot();

//--------------------------------------------------------------------------------
// Events
//--------------------------------------------------------------------------------
			/**
			 * @brief Updates the scene during runtime
			 * @return void
			 *
			 * This should be called each frame just before the window is refreshed.
			**/
		void OnUpdate(TimeStep dt);

			/**
			 * @brief Updates scene cameras to reflect the new aspect ratio
			 * @param[in] width of scene
			 * @param[in] height of scene
			 * @return void
			**/
		void OnViewportResize(unsigned int width, unsigned int height);

//--------------------------------------------------------------------------------
// Simulation
//--------------------------------------------------------------------------------
			/**
			 * @brief Resumes scene simulation
			 * @retval void
			**/
		void Start();

			/**
			 * @brief Pauses scene simulation
			 * @retval void
			**/
		void Stop();

			/**
			 * @brief Returns running state of simulation
			 * @retval true if simulation is running
			 * @retval false if simulation is **not** running
			**/
		bool IsRunning();

			/**
			 * @brief Sets the active camera for the scene
			 * @param[in] entityTag of entity with CameraComponent
			 * @retval true if active camera was updated
			 * @retval false if active camera was **not** updated
			 *
			 * Searches for an entity with a camera that matches \p entityTag,
			 * the active camera is only changed if a matching entity if found.
			**/
		bool SetActiveCamera(const std::string& entityTag);

//--------------------------------------------------------------------------------
// Debug Camera
//--------------------------------------------------------------------------------
			/**
			 * @brief Sets whether the scene uses the debug camera or an entity camera
			 * @param[in] value true to use debug cam; false to revert to entity cam
			 * @return void
			 * @bug Currently controllable entities are still updated
			**/
		void UseDebugCamera(bool value);

			/**
			 * @brief Returns status of Scene::SetDebugCamera
			 * @retval true if using debug camera
			 * @retval false if **not** using debug camera
			**/
		bool UsingDebugCamera() const;

			/**
			 * @brief Returns the debug camera reference to update properties
			 * @return DebugCamera&
			**/
		DebugCamera& GetDebugCamera();

	private:
		friend class Entity;
		friend class SceneSerialiser;

		// core
		std::string m_ident;				///< Scene identifier
		entt::registry m_Registry;			///< EnTT registry for this scene
		bool m_isRunning{ false };
		PhysicsWorld* m_physicsWorld;

		// snapshots
		std::stack<Memento> m_snapshots;

		// debugging
		DebugCamera m_debugCam;				///< Scene debug camera
		bool m_useDebugCamera{ false };		///< Scene debug camera active state

			/**
			 * @brief Calls modern render system with the given camera
			 * @param[in] activeCam to render scene from
			 * @return void
			**/
		void RenderOnUpdate(const PerspectiveCamera& activeCam);

		/**
			 * @brief Initialises scene
			 * @retval void
			**/
		void Init(unsigned int updatesPerSecond = 60);

		void TerrainOnUpdate(const PerspectiveCamera& activeCam);

			/**
			 * @brief Updates cameras in scene
			 * @retval PerspectiveCamera* if there is an active camera
			 * @retval nullptr is there was no active camera
			 * @warning If function returns nullptr, the program will terminate
			 * @note For optimisation reasons, only the active camera will have
			 * its view matrix updated.
			**/
		PerspectiveCamera* CamerasOnUpdate();

		void PhysicsOnUpdate();

		/*******************************************
		* Scripting Testing functions
		*******************************************/

		void ScriptableOnUpdate(TimeStep dt);
	};
}
