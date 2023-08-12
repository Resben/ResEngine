/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once
#include <assimp/anim.h>

#include "AEngine/Render/Bone.h"

namespace AEngine
{

		/**
		 * @struct Bone
		 * @brief Stores animation data for a Bone
		**/
	class AssimpBone : public Bone
	{
	public:
			/**
			 * @brief Contructor
			 * @param[in] animNode Assimp aiNodeAnim object
			**/
		AssimpBone(const aiNodeAnim* animNode);

	private:
			/**
			 * @brief Load bone keyframes
			 * @param[in] animNode Assimp animNode object
			**/
		void LoadData(const aiNodeAnim* animNode);
	};
}