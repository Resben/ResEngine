#include "Math.h"

namespace AEngine
{
	namespace Math
	{
		Math::vec3 rotateVec(const Math::vec3& vector, const Math::quat& orientation)
		{
			Math::mat3 rotationMatrix = Math::mat3_cast(orientation);
			return rotationMatrix * vector;
		}
	}
}
