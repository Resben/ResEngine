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
		std::shared_ptr<T> Load(const std::string& path);
		std::shared_ptr<T> Get(const std::string& ident);

		typename std::map<std::string, typename std::shared_ptr<T>>::const_iterator begin();
		typename std::map<std::string, typename std::shared_ptr<T>>::const_iterator end();

		AssetManager(const AssetManager<T>&) = delete;
		void operator=(const AssetManager<T>&) = delete;

	private:
		AssetManager() = default;
		std::map<std::string, typename std::shared_ptr<T>> m_data;
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
	std::shared_ptr<T> AssetManager<T>::Load(const std::string& path)
	{
		std::size_t last = path.find_last_of("/");
		const std::string ident = path.substr(last + 1);

		m_data.emplace(std::make_pair(
			ident, T::Create(ident, path))
		);

		AE_LOG_TRACE("AssetManager::Load::Success -> {}", name);
		return m_data[ident];
	}

	template <typename T>
	std::shared_ptr<T> AssetManager<T>::Get(const std::string& ident)
	{
		std::map<std::string, typename std::shared_ptr<T>>::iterator it;
		it = m_data.find(ident);
		if (it != m_data.end())
			return it->second;
		else
			return nullptr;
	}

	template <typename T>
	typename std::map<std::string, typename std::shared_ptr<T>>::const_iterator AssetManager<T>::begin()
	{
		return m_data.begin();
	}

	template <typename T>
	typename std::map<std::string, typename std::shared_ptr<T>>::const_iterator AssetManager<T>::end()
	{
		return m_data.end();
	}
}