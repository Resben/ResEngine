/**
 * \file
 * \author Ben Hawkins (34112619)
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
		 * \class AssimpModel
		 * \brief Contains implementation to load Model data using Assimp
		**/
	class AssimpModel : public Model
	{
	public:
			/**
			 * \brief Contructor loads asset by path
			 * \param[in] ident Asset ident
			 * \param[in] path Asset path
			**/
		AssimpModel(const std::string& ident, const std::string& path);
		
		virtual ~AssimpModel();

	private:
			/**
			 * \brief Recursively processes nodes to load meshes
			 * \param[in] node Assimp node
			 * \param[in] scene Assimp scene to load from
			**/
		void ProcessNode(const aiNode* node, const aiScene* scene);
			/**
			 * \brief Create a mesh and instantiate a material
			 * \param[in] scene Assimp scene to load from
			 * \param[in] mesh Assimp mesh to read from
			 * \return mesh_material A mesh to material ID pair
			**/
		mesh_material CreateMesh(const aiScene* scene, const aiMesh* mesh);
			/**
			 * \brief Maps bone names to a int ID
			 * \param[in] name Bone name
			 * \param[in] bone Bone to load info from
			**/
		int NameToID(std::string& name, const aiBone* bone);
			/**
			 * \brief Loads bone vertex weights and ids
			 * \param[in] mesh Assimp mesh object to load from
			 * \param[in] boneWeights vector to store boneweights
			 * \param[in] boneIDs vector to store boneids
			**/
		void LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);
	};
}