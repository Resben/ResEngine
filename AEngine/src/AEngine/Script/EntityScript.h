#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"
#include "ScriptEnvironment.h"

namespace AEngine
{
	class Entity;

		/**
		 * @brief Entity attachable script
		 * @author Geoff Candy (34183006)
		**/
	class EntityScript : public Asset
	{
	public:
		/**
		 * @brief Script Constructor for the Script
		 * @param std::string, std::string
		 **/
		EntityScript(ScriptState& state, const std::string& ident, const std::string& fname);
		
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
		static SharedPtr<EntityScript> Create(const std::string& ident, const std::string& fname);
	
	private:
		ScriptEnvironment m_env;
	};
}
