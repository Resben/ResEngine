#pragma once
#include <map>
#include <memory>
#include <string>
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	template <typename T>
	class AssetManager
	{
	public:
		~AssetManager();

		static AssetManager<T>& Instance();
		void Clear();
		SharedPtr<T> LoadSubAsset(const std::string& ident, const SharedPtr<T> assetToCopy);
		SharedPtr<T> Load(const std::string& path);
		SharedPtr<T> Get(const std::string& ident);

		typename std::map<std::string, typename SharedPtr<T>>::const_iterator begin();
		typename std::map<std::string, typename SharedPtr<T>>::const_iterator end();

		AssetManager(const AssetManager<T>&) = delete;
		void operator=(const AssetManager<T>&) = delete;

	private:
		AssetManager() = default;
		std::map<std::string, typename SharedPtr<T>> m_data;
	};

//--------------------------------------------------------------------------------
// Implementation
//--------------------------------------------------------------------------------

	template <typename T>
	AssetManager<T>::~AssetManager()
	{
		AE_LOG_INFO("AssetManager::Clear");
		Clear();
	}

	template <typename T>
	AssetManager<T>& AssetManager<T>::Instance()
	{
		static AssetManager<T> instance;
		return instance;
	}

	template <typename T>
	void AssetManager<T>::Clear()
	{
		m_data.clear();
	}

	template <typename T>
	SharedPtr<T> AssetManager<T>::Load(const std::string& path)
	{
		Size_t last = path.find_last_of("/");
		const std::string ident = path.substr(last + 1);

		// test if exists, and create if doesn't
		SharedPtr<T> obj = Get(ident);
		if (!obj)
		{
			m_data.emplace(std::make_pair(
				ident, T::Create(ident, path))
			);
			obj = Get(ident);
		}
	
		AE_LOG_TRACE("AssetManager::Load::Success -> {}", name);
		return obj;
	}

		// Copies an asset to the manager 
		// Used when T::Create() is not possible
	template <typename T>
	SharedPtr<T> AssetManager<T>::LoadSubAsset(const std::string& ident, const SharedPtr<T> assetToCopy)
	{
		SharedPtr<T> obj = Get(ident);
		if (!obj)
		{
			m_data.emplace(std::make_pair(
				ident, assetToCopy)
			);
			obj = Get(ident);
		}
	
		AE_LOG_TRACE("AssetManager::Load::Success -> {}", name);
		return obj;
	}

	template <typename T>
	SharedPtr<T> AssetManager<T>::Get(const std::string& ident)
	{
		std::map<std::string, typename SharedPtr<T>>::iterator it;
		it = m_data.find(ident);
		if (it != m_data.end())
			return it->second;
		else
			return nullptr;
	}

	template <typename T>
	typename std::map<std::string, typename SharedPtr<T>>::const_iterator AssetManager<T>::begin()
	{
		return m_data.begin();
	}

	template <typename T>
	typename std::map<std::string, typename SharedPtr<T>>::const_iterator AssetManager<T>::end()
	{
		return m_data.end();
	}
}
