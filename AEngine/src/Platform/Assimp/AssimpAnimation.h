/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#pragma once

#include <assimp/scene.h>

#include "AEngine/Render/Animation.h"
#include "AssimpBone.h"

namespace AEngine
{
		/**
		 * \class AssimpAnimation
		 * \brief Contains implementation to load animation using Assimp
		**/
	class AssimpAnimation : public Animation
	{
	public:
		virtual ~AssimpAnimation() = default;
			/**
			 * \brief Constructor to create a Animation from an Assimp object
			 * \param[in] ident Asset ident
			 * \param[in] parent Asset path
			 * \param[in] scene Assimp scene to load from
			 * \param[in] bone_map Bone map created in model
			 * \param[in] animationIndex Index of animation to be loaded
			*/
		AssimpAnimation(const std::string& ident, const std::string& parent, const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex);

	private:

			/**
			 * \brief Load Assimp aiNode structure from root node into SceneNode structure recursively
			 * \param[in] node SceneNode to load too
			 * \param[in] src aiNode to load from
			**/
		void ProcessNode(SceneNode& node, const aiNode* src);
	};
}
