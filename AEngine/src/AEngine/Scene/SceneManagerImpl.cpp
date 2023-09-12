#include "SceneManagerImpl.h"
#include "SceneSerialiser.h"

namespace AEngine
{
	SceneManagerImpl& SceneManagerImpl::Instance()
	{
		static SceneManagerImpl instance;
		return instance;
	}

	Scene* SceneManagerImpl::CreateScene(const std::string& ident)
	{
		// check if scene already exists
		if (HasScene(ident))
		{
			return nullptr;
		}

		// creates a new scene and adds it to m_scenes
		// done this way to enable privatising Scene constructor
		UniquePtr<Scene> scene(new Scene(ident));
		m_scenes[ident] = std::move(scene);
		return GetScene(ident);
	}

	Scene* SceneManagerImpl::LoadFromFile(const std::string& path)
	{
		// serialise scene from file and check if it was successful
		UniquePtr<Scene> scene = SceneSerialiser::DeserialiseFile(path);
		if (!scene)
		{
			return nullptr;
		}

		// check if scene already exists
		std::string ident = scene->GetIdent();
		if (HasScene(ident))
		{
			return nullptr;
		}

		// add scene to m_scenes
		m_scenes[ident] = std::move(scene);
		return GetScene(ident);
	}

	bool SceneManagerImpl::SaveActiveToFile(const std::string& path)
	{
		// check if there is an active scene
		if (!m_activeScene)
		{
			return false;
		}

		// serialise active scene to file
		SceneSerialiser::SerialiseFile(m_activeScene, path);
		return true;
	}

	bool SceneManagerImpl::UnloadScene(const std::string& ident)
	{
		if (!HasScene(ident))
		{
			return false;
		}

		m_scenes.erase(ident);
		return true;
	}

	void SceneManagerImpl::UnloadAllScenes()
	{
		m_scenes.clear();
	}

	std::vector<std::string> SceneManagerImpl::GetSceneIdents() const
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

	bool SceneManagerImpl::HasScene(const std::string& ident) const
	{
		return m_scenes.find(ident) != m_scenes.end();
	}

	bool SceneManagerImpl::SetActiveScene(const std::string& ident)
	{
		if (!HasScene(ident))
		{
			return false;
		}

		m_activeScene = m_scenes[ident].get();
		return true;
	}

	Scene* SceneManagerImpl::GetActiveScene() const
	{
		return m_activeScene;
	}

	Scene *SceneManagerImpl::GetScene(const std::string &ident) const
	{
		auto it = m_scenes.find(ident);
		if (it == m_scenes.end())
		{
			return nullptr;
		}

		return it->second.get();
	}
}
