#pragma once
#include "AEngine/Core/Types.h"
#include "ScriptEnvironment.h"
#include "Script.h"

namespace AEngine
{
	class Entity;

		/**
		 * @brief Entity attachable script
		 * @author Geoff Candy (34183006)
		**/
	class EntityScript
	{
	public:
		/**
		 * @brief Script Constructor for the Script
		 * @param std::string, std::string
		 **/
		EntityScript(ScriptState& state, const Script* script);
		void SetEntity(Entity* entity);

		/**
		 * @brief OnUpdate Method called from within lua scripts
		 * @param float, Entity
		 **/
		void OnUpdate(float deltaTime);

		const std::string& GetIdent() const;
		const std::string& GetPath() const;

	private:
		ScriptEnvironment m_env;
		const Script* m_script;

		/**
		 * @brief OnUpdate Method called from within lua scripts
		 **/
		void OnStart();

		/**
		 * @brief OnDestroy Method called from within lua scripts
		 **/
		void OnDestroy();
	};
}
