#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace AEngine
{
	namespace Math
	{
		using namespace glm;

		vec3 rotateVec(const vec3& vector, const quat& orientation);
	}
}
