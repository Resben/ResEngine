/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once

#include "AEngine/Render/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define MAX_BONE_INFLUENCE 4

namespace AEngine
{
		/**
		 * @class Model
		 * @brief Model object, contains material and animations
		**/
	class AssimpModel : public Model
	{
	public:
		AssimpModel(const std::string& ident, const std::string& path);
		
		virtual ~AssimpModel();

	private:

		void ProcessNode(const aiNode* node, const aiScene* scene);

		mesh_material CreateMesh(const aiScene* scene, const aiMesh* mesh);

		int NameToID(std::string& name, const aiBone* bone);

		void LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);
	};
}