/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include <vector>
#include <string>

#include "AEngine/Math/Math.h"

namespace AEngine
{
		/**
		 * @struct KeyPosition
		 * @brief Stores a position keyframe
		**/
	struct KeyPosition
	{
		Math::vec3 position;
		float timeStamp;
	};

		/**
		 * @struct KeyRotation
		 * @brief Stores a rotation keyframe
		**/
	struct KeyRotation
	{
		Math::quat rotation;
		float timeStamp;
	};

		/**
		 * @struct KeyScale
		 * @brief Stores a scale keyframe
		**/
	struct KeyScale
	{
		Math::vec3 scale;
		float timeStamp;
	};

		/**
		 * @struct Bone
		 * @brief Stores animation data for a Bone
		**/
	class Bone
	{
	public:
			/**
			 * @brief Contructor
			 * @param[in] animNode Assimp aiNodeAnim object
			**/
		Bone() = default;
			/**
			 * @brief Update and return a bone transform
			 * @param[in] animationTime current time in animation
			**/
		virtual const Math::mat4 GetLocalTransform(float animationTime) const = 0;
			/**
			 * @brief Return a bone name
			 * @return String bone name
			**/
		virtual std::string GetBoneName() const = 0;

	};
}