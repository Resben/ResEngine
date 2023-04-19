#pragma once

#include <iostream>

#include "AEngine/Script/ScriptState.h"

namespace AEngine
{
	class Entity;

	class ScriptComponent
	{
	public:
		ScriptComponent(const std::string& scriptFileName);


		void OnStart();
		void OnUpdate(float deltaTime, Entity entity);
		void OnDestroy();

	private:
		ScriptState m_state;
	};
}
