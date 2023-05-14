#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Script/ScriptState.h"

namespace AEngine
{
	class Entity;

		/**
		 * @class Scene
		 * @brief Script Class for the script 
		 * @author Geoff Candy (34183006)
		**/
	class Script : public Asset
	{
	public:
		
		/**
		 * @brief Script Constructor for the Script
		 * @param std::string, std::string
		 **/
		Script(const std::string& ident, const std::string& fname);

		/**
		 * @brief OnUpdate Method called from within lua scripts
		 **/
		void OnStart();

		/**
		 * @brief OnUpdate Method called from within lua scripts
		 * @param float, Entity
		 **/
		void OnUpdate(float deltaTime, Entity entity);
		
		/**
		 * @brief OnDestroy Method called from within lua scripts
		 **/
		void OnDestroy();
		
		/**
		 * @brief Create Method to create the Script pointer used in Components 
		 **/
		static SharedPtr<Script> Create(const std::string& ident, const std::string& fname);
	
	private:
		ScriptState m_state;
	};
}
