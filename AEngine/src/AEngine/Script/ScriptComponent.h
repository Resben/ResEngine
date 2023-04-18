#pragma once

#include <iostream>

#include "AEngine/Script/ScriptState.h"

class ScriptComponent
{
public:
	ScriptComponent(const std::string& scriptFileName);


	void OnStart();
	void OnUpdate(float deltaTime);
	void OnDestroy();

private:
	ScriptState m_state;
};