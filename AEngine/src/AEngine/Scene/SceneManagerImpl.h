/**
 * \file
 * \author Christien Alden (34119981)
 * \brief SceneManagerImpl header file
*/
#pragma once
#include <map>
#include <vector>

#include "Scene.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{
		/**
		 * \class SceneManagerImpl
		 * \brief Manages the scenes in the game
		 * \author Christien Alden (34119981)
		*/
	class SceneManagerImpl
	{
	public:
			/**
			 * \brief Gets the instance of the SceneManagerImpl
			 * \return The instance of the SceneManagerImpl
			*/
		static SceneManagerImpl& Instance();

			/**
			 * \copydoc SceneManager::CreateScene
			*/
		Scene* CreateScene(const std::string& ident);
			/**
			 * \copydoc SceneManager::LoadFromFile
			*/
		Scene* LoadFromFile(const std::string& path);
			/**
			 * \copydoc SceneManager::SaveActiveToFile
			*/
		bool SaveActiveToFile(const std::string& path);
			/**
			 * \copydoc SceneManager::UnloadScene
			*/
		bool UnloadScene(const std::string& ident);
			/**
			 * \copydoc SceneManager::UnloadAllScenes
			*/
		void UnloadAllScenes();
			/**
			 * \copydoc SceneManager::GetSceneIdents
			*/
		std::vector<std::string> GetSceneIdents() const;
			/**
			 * \copydoc SceneManager::HasScene
			*/
		bool HasScene(const std::string& ident) const;

			/**
			 * \copydoc SceneManager::SetActiveScene
			*/
		bool SetActiveScene(const std::string& ident);
			/**
			 * \copydoc SceneManager::GetActiveScene
			*/
		Scene* GetActiveScene() const;
			/**
			 * \copydoc SceneManager::GetScene
			*/
		Scene* GetScene(const std::string& ident) const;

	private:
		SceneManagerImpl() = default;
		Scene* m_activeScene;
		std::map<std::string, UniquePtr<Scene>> m_scenes;
	};
}
