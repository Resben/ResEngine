#pragma once
#include <EnTT/entt.hpp>
#include "AEngine/Core/Logger.h"
#include "Scene.h"

namespace AEngine
{
		/**
		 * @class Entity
		 * @brief Entity Class used by the ECS
		 * @author Geoff Candy (34183006)
		 * @version 01
		 * @date 04/09/2022
		**/
	class Entity
	{
	public:
		Entity() = default;
			/**
			 * @brief Constructor for Entity class
			**/
		Entity(entt::entity handle, Scene* scene);

			/**
			 * @brief Default Copy Constructor for Entity Class
			**/
		Entity(const Entity& other) = default;

			/**
			 * @brief Method to add a component to the entity
			 * forwards Args to EnTT
			 * @param T = component name, Args = initial values of component
			**/
		template<typename T, typename ...Args>
		T* AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				return nullptr;
			}

			return &m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename ...Args>
		T* ReplaceComponent(Args&&... args)
		{
			return &m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

			/**
			 * @brief Method to return a component from an Entity
			**/
		template<typename T>
		T* GetComponent()
		{
			if (!HasComponent<T>())
			{
				return nullptr;
			}

			return &m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

			/**
			 * @brief Method to remove a component from an Entity
			**/
		template<typename T>
		void RemoveComponent()
		{
			if (HasComponent<T>())
			{
				m_Scene->m_Registry.remove<T>(m_EntityHandle);
			}
		}

			/**
			 * @brief Method to check whether an Entity has a Component
			**/
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		void Destroy()
		{
			m_Scene->m_entitiesStagedForRemoval.push_back(m_EntityHandle);
		}

		Scene* GetScene() { return m_Scene; }

		// utility methods and overloads
		bool operator==(const Entity& rhs) { return Equal(rhs); }
		bool operator!=(const Entity& rhs) { return !Equal(rhs); }
		operator bool() { return Valid(); }
		bool IsValid() { return Valid(); }

	private:
		entt::entity m_EntityHandle{ entt::null };	///< Entity handle
		Scene* m_Scene{ nullptr };					///< Scene this entity belongs to

		bool Valid() const
		{
			return {
				m_EntityHandle != entt::null &&
				m_Scene != nullptr
			};
		}

		bool Equal(const Entity& other) const
		{
			return {
				m_EntityHandle == other.m_EntityHandle &&
				m_Scene == other.m_Scene
			};
		}
	};
}
