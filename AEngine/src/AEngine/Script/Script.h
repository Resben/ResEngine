#pragma once

#include <iostream>

#include "AEngine/Script/ScriptState.h"
#include "AEngine/Resource/Asset.h"

namespace AEngine
{
	class Entity;

	class Script : public Asset
	{
	public:
		Script(const std::string& ident, const std::string& fname);

		void OnStart();
		void OnUpdate(float deltaTime, Entity entity);
		void OnDestroy();
		
		static std::shared_ptr<Script> Create(const std::string& ident, const std::string& fname);
	
	private:
		ScriptState m_state;
	};
}
