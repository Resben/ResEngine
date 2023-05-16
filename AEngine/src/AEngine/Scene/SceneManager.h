/**
 * \file
 * \author Christien Alden (34119981)
 * \brief SceneManager header file
*/
#pragma once
#include <map>
#include <vector>

#include "Scene.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{
		/**
		 * \class SceneManager
		 * \brief Manages the scenes in the game
		 * \author Christien Alden (34119981)
		*/
	class SceneManager
	{
	public:
		static SceneManager& Instance();

		Scene* LoadScene(UniquePtr<Scene> scene);
		bool UnloadScene(const std::string& ident);
		void UnloadAllScenes();
		std::vector<std::string> GetSceneIdents() const;
		bool HasScene(const std::string& ident) const;

		bool SetActiveScene(const std::string& ident);
		Scene* GetActiveScene() const;
		Scene* GetScene(const std::string& ident) const;

	private:
		SceneManager() = default;
		Scene* m_activeScene;
		std::map<std::string, UniquePtr<Scene>> m_scenes;
	};
}
