/**
 * @file
 * @author Ben Hawkins
 * @date 12/09/2022
*/
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/ModelManager.h"

#include <vector>

namespace AEngine
{
	ModelManager* ModelManager::s_instance = nullptr;

	ModelManager* ModelManager::Instance()
	{
		if (!s_instance)
		{
			s_instance = new ModelManager;
		}

		return s_instance;
	}

	void ModelManager::Clear()
	{
		AE_LOG_DEBUG("ModelManager::Clear");
		m_models.clear();
	}

	void ModelManager::LoadModel(const std::string& filePath)
	{
		std::string filename;
		std::size_t last = filePath.find_last_of("/");

		if (last != std::string::npos)
			filename = filePath.substr(last + 1);

		m_models.emplace(std::make_pair(filename, std::make_shared<Model>(filePath)));

		AE_LOG_TRACE("ModelManager::Load::Success -> {}", filePath);
	}

	Model* ModelManager::GetModel(const std::string& modelName)
	{
		std::map<std::string, std::shared_ptr<Model>>::iterator it;
		it = m_models.find(modelName);
		if (it != m_models.end())
			return it->second.get();
		else
			return nullptr;
	}
}
