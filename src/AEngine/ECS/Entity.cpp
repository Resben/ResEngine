/**
 * @file
 * @author Geoff Candy (34183006)
 * @brief Entity class implementation
**/
#include "AEngine/ECS/Entity.h"

namespace AEngine
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}
