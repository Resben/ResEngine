#include "AEngine/Physics/ReactManager.h"

using namespace React;

ReactManager::ReactManager() :
	worldPtr(nullptr),
	m_timestep(0),
	m_accumulator(0)
{

}

ReactManager& ReactManager::GetInstance()
{
	static ReactManager instance;

	return instance;
}

void ReactManager::Init(float timeStep)
{
	worldPtr = common.createPhysicsWorld();
	m_timestep = timeStep;

	worldPtr->setEventListener(&m_listener);
}

void ReactManager::Update(float dtSeconds)
{
	m_accumulator += dtSeconds;

	while (m_accumulator >= m_timestep)
	{
		worldPtr->update(m_timestep);
		m_accumulator -= m_timestep;
	}
}

float ReactManager::GetInterpolationFactor()
{
	return m_accumulator / m_timestep;
}

PhysicsEventListener& ReactManager::GetPhysicsEventListener()
{
	return m_listener;
}