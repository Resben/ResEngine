/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#pragma once
#include <assimp/anim.h>

#include "AEngine/Render/Bone.h"

namespace AEngine
{

		/**
		 * \class AssimpBone
		 * \brief Contains implementation to load Bone data using Assimp
		**/
	class AssimpBone : public Bone
	{
	public:
			/**
			 * \brief Contructor
			 * \param[in] animNode Assimp aiNodeAnim to load from
			**/
		AssimpBone(const aiNodeAnim* animNode);

	private:
			/**
			 * \brief Load bone keyframes
			 * \param[in] animNode Assimp animNode to load from
			**/
		void LoadData(const aiNodeAnim* animNode);
	};
}