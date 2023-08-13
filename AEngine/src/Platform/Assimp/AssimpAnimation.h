/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include <assimp/scene.h>

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
		AssimpAnimation(const std::string& ident, const std::string& parent, const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex);

	private:

			/**
			 * @brief Load Assimp aiNode structure from root node into SceneNode structure recursively
			 * @param[in] node SceneNode to load too
			 * @param[in] src aiNode to load from
			**/
		void ProcessNode(SceneNode& node, const aiNode* src);
	};
}
