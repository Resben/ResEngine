#include "SceneManager.h"

namespace AEngine
{
	SceneManager& SceneManager::Instance()
	{
		static SceneManager instance;
		return instance;
	}

	Scene* SceneManager::LoadScene(UniquePtr<Scene> scene)
	{
		std::string ident = scene->GetIdent();
		if (HasScene(ident))
		{
			return nullptr;
		}

		/// \todo Check return value of this
		m_scenes[ident] = std::move(scene);
		return GetScene(ident);
	}

	bool SceneManager::UnloadScene(const std::string& ident)
	{
		if (!HasScene(ident))
		{
			return false;
		}

		m_scenes.erase(ident);
		return true;
	}

	void SceneManager::UnloadAllScenes()
	{
		m_scenes.clear();
	}

	std::vector<std::string> SceneManager::GetSceneIdents() const
	{
		if (m_scenes.empty())
		{
			return std::vector<std::string>();
		}

		// iterate through m_scenes and generate a vector of identifiers
		std::vector<std::string> idents;
		idents.reserve(m_scenes.size());

		for (const auto& scene : m_scenes)
		{
			idents.push_back(scene.first);
		}

		return idents;
	}

	bool SceneManager::HasScene(const std::string& ident) const
	{
		return m_scenes.find(ident) != m_scenes.end();
	}

	bool SceneManager::SetActiveScene(const std::string& ident)
	{
		if (!HasScene(ident))
		{
			return false;
		}

		m_activeScene = m_scenes[ident].get();
		return true;
	}

	Scene* SceneManager::GetActiveScene() const
	{
		return m_activeScene;
	}

	Scene *SceneManager::GetScene(const std::string &ident) const
	{
		auto it = m_scenes.find(ident);
		if (it == m_scenes.end())
		{
			return nullptr;
		}

		return it->second.get();
	}
}
