/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include <vector>
#include <map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Bone.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"

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
	class Animation
	{
	public:
			/// @brief Deconstructor
		~Animation();
			/// @brief Constructor
		Animation(const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex);

		const std::string& GetName() const; 
		const float GetDuration() const;
		const float GetTicksPerSecond() const;
		const std::vector<Bone>& GetBones() const;
		const std::map<std::string, BoneInfo>& GetBoneMap() const;
		const SceneNode& GetRoot() const;

	private:

			/**
			 * @brief Load Assimp aiNode structure from root node into SceneNode structure recursively
			 * @param[in] node SceneNode to load too
			 * @param[in] src aiNode to load from
			**/
		void ProcessNode(SceneNode& node, const aiNode* src);

			/// @brief Root node of SceneNode data
		SceneNode m_RootNode;
			/// @brief Map bone name to info
		std::map<std::string, BoneInfo> m_BoneInfoMap;

		std::string m_name;
		float m_duration;
		float m_ticksPerSecond;
		std::vector<Bone> m_bones;
	};
}