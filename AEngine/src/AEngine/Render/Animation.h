/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include <vector>
#include <map>
#include <string>

#include "Bone.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{
	struct AnimationData
	{
		std::string name;
		float duration;
		float ticksPerSecond;
		float currentTime;
		float lastTime = -1.0f;
	};

		/**
		 * @struct BoneInfo
		 * @brief Stores a bone ID and local offset
		**/
	struct BoneInfo
	{
		int id;
		Math::mat4 offset;
	};

		/**
		 * @struct SceneNode
		 * @brief Stores Assimp node data
		**/
	struct SceneNode
	{
		Math::mat4 transformation;
		std::string name;
		int numChildren;
		std::vector<SceneNode> children;
	};

		/**
		 * @class Animation
		 * @brief Contains all of the animations of a model
		**/
	class Animation : public Asset
	{
	public:
			/// @brief Deconstructor
		virtual ~Animation() = default;
			/// @brief Constructor
		Animation(const std::string& ident, const std::string& path);

		virtual const std::string& GetName() const = 0; 
		virtual const float GetDuration() const = 0;
		virtual const float GetTicksPerSecond() const = 0;
		virtual const std::vector<SharedPtr<Bone>>& GetBones() const = 0;
		virtual const std::map<std::string, BoneInfo>& GetBoneMap() const = 0;
		virtual const SceneNode& GetRoot() const = 0;

		static SharedPtr<Animation> Create(const std::string& ident, const std::string& parent, const SharedPtr<Animation> asset);
	};
}