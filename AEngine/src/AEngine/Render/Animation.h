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

namespace AEngine
{

		/**
		 * @struct AnimationData
		 * @brief Stores a single animation
		**/
	struct AnimationData
	{
		float duration;
		float ticksPerSecond;
		float currentTime;
		float lastTime = -1.0f;
		std::vector<Bone> bones;
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
		Animation();

		std::string GetName();
			/**
			 * @brief Load a animation from a aiScene
			 * @param[in] scene Assimp aiScene object
			 * 
			 * @todo Remove testing code
			**/
		void Load(const aiScene* scene);
			/**
			 * @brief Update an animation
			 * @param[in] dt Delta time
			**/
		void UpdateAnimation(float dt);
			/**
			 * @brief Get bone transforms (up to 100 bones)
			 * @return vector of transform matrices
			 * 
			 * @note Must call UpdateAnimation beforehand
			**/
		std::vector<Math::mat4>& GetFinalBoneMatrices();

	private:

			/**
			 * @brief Map bones to name
			 * @param[in] scene Assimp aiScene object
			**/
		void MapBones(const aiScene* scene);
			/**
			 * @brief Load Assimp aiNode structure from root node into SceneNode structure recursively
			 * @param[in] node SceneNode to load too
			 * @param[in] src aiNode to load from
			**/
		void ProcessNode(SceneNode& node, const aiNode* src);
			/**
			 * @brief Calculate all bone transformations recursively
			 * @param[in] node SceneNode
			 * @param[in] parentTransform transform of parent SceneNode
			**/
		void CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform);
			/**
			 * @brief Get a Bone object from vector of current animation
			 * @param[in] name string name of bone
			**/
		Bone* GetBone(const std::string& name);

			/// @brief Is model animated
		bool isAnimated;
			/// @brief Vector of bone matrices
		std::vector<Math::mat4> m_FinalBoneMatrices;
			/// @brief Root node of SceneNode data
		SceneNode m_RootNode;
			/// @brief Map bone name to info
		std::map<std::string, BoneInfo> m_BoneInfoMap;

		std::string m_name;
		float m_duration;
		float m_ticksPerSecond;
		float m_currentTime;
		float m_lastTime = -1.0f;
		std::vector<Bone> m_bones;
	};
}