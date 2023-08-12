/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEngine/Render/Animation.h"
#include "AssimpBone.h"

namespace AEngine
{
		/**
		 * @class Animation
		 * @brief Contains all of the animations of a model
		**/
	class AssimpAnimation : public Animation
	{
	public:
			/// @brief Deconstructor
		virtual ~AssimpAnimation();
			/// @brief Constructor
	    AssimpAnimation(const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex);
        AssimpAnimation(const std::string& ident, const std::string& parent, const SharedPtr<Animation> animation);

		const std::string& GetName() const; 
		const float GetDuration() const;
		const float GetTicksPerSecond() const;
		const std::vector<SharedPtr<Bone>>& GetBones() const;
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
		std::vector<SharedPtr<Bone>> m_bones;
	};
}
