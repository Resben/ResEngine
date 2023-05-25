#pragma once
#include "AEngine/Core/Types.h"
#include "Script.h"
#include "ScriptEnvironment.h"

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
		EntityScript(Entity& entity, ScriptState& state, const Script* script);

		template <typename T>
		void SetLocal(const std::string& name, T value);

		/**
		 * @brief OnUpdate Method called from within lua scripts
		 * @param float, Entity
		 **/
		void OnUpdate(float deltaTime);
		void OnFixedUpdate(float deltaTime);
		void OnLateUpdate(float deltaTime);

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

	template <typename T>
	void EntityScript::SetLocal(const std::string& name, T value)
	{
		m_env.SetLocal(name, value);
	}
}
