#include "SceneManager.h"
#include "SceneManagerImpl.h"

namespace AEngine
{
	Scene* SceneManager::CreateScene(const std::string& ident)
	{
		return SceneManagerImpl::Instance().CreateScene(ident);
	}

	bool SceneManager::UnloadScene(const std::string& ident)
	{
		return SceneManagerImpl::Instance().UnloadScene(ident);
	}

	void SceneManager::UnloadAllScenes()
	{
		SceneManagerImpl::Instance().UnloadAllScenes();
	}

	std::vector<std::string> SceneManager::GetSceneIdents()
	{
		return SceneManagerImpl::Instance().GetSceneIdents();
	}

	bool SceneManager::HasScene(const std::string& ident)
	{
		return SceneManagerImpl::Instance().HasScene(ident);
	}

	bool SceneManager::SetActiveScene(const std::string& ident)
	{
		return SceneManagerImpl::Instance().SetActiveScene(ident);
	}

	Scene* SceneManager::GetActiveScene()
	{
		return SceneManagerImpl::Instance().GetActiveScene();
	}

	Scene* SceneManager::GetScene(const std::string& ident)
	{
		return SceneManagerImpl::Instance().GetScene(ident);
	}
}