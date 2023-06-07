/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once
#include <utility>
#include <string>
#include <vector>
#include <map>

#include "AEngine/Core/Types.h"
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Resource/Asset.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Animation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONE_INFLUENCE 4

namespace AEngine
{
		/**
		 * @struct Material
		 * @brief Stores a mesh's material
		**/
	struct Material
	{
		std::string DiffuseTexture;
		std::string SpecularTexture;
	};

		/**
		 * @class Model
		 * @brief Model object, contains material and animations
		**/
	class Model : public Asset
	{
	public:
		using mesh_material = std::pair<SharedPtr<Mesh>, int>;
			/**
			 * @brief Constructor
			 * @param[in] ident Asset ident
			 * @param[in] path Asset path
			**/
		Model(const std::string& ident, const std::string& path);
			/**
			 * @brief Clear model data
			**/
		void Clear();
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, const TimeStep time);
		const SharedPtr<Mesh>& GetMesh(int index) const;
			/**
			 * @brief Get material for a mesh by index
			 * @param[in] meshIndex Index of mesh
			**/
		const Material* GetMaterial(int meshIndex) const;
			/**
			 * @brief Get animation object for model
			 * @return Animation reference
			**/
		Animation& GetAnimation();
			/**
			 * @brief Deconstructor 
			**/
		~Model();

		std::vector<mesh_material>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<mesh_material>::const_iterator end() const { return m_meshes.end(); }

		static SharedPtr<Model> Create(const std::string& ident, const std::string& fname);

	private:
			/**
			 * @brief Loads textures from Assimp structures
			 * @param[in] mat Assimp material object
			 * @param[in] type Assimp texture type
			 * @return string id
			 * 
			 * @note Only single texture type supported
			**/
		std::string LoadTextures(aiMaterial* mat, aiTextureType type);
			/**
			 * @brief Loads materials from Assimp structures
			 * @param[in] scene Assimp aiScene object
			**/
		void GenerateMaterials(const aiScene* scene);
			/**
			 * @brief Recursively loads meshes from Assimp structures
			 * @param[in] scene Assimp aiScene object
			**/
		void ProcessNode(aiNode* node, const aiScene* scene);
			/**
			 * @brief Creates a mesh from Assimp structures
			 * @param[in] mesh Assimp aiMesh object
			 * @return Mesh and material pair
			**/
		mesh_material CreateMesh(aiMesh* mesh);
			/**
			 * @brief Maps assimp bone names to a ID
			 * @param[in] string name of bone
			 * @param[in] bone Assimp aiBone object
			 * @return ID for bone name
			**/
		int NameToID(std::string& name, aiBone* bone);
			/**
			 * @brief Loads Assimp bone data into vectors
			 * @param[in] mesh Assimp aiMesh object
			 * @param[in] BoneWeights Vector of floats for weight data
			 * @param[in] BoneIDs Vector of ints for ID data
			**/
		void LoadMeshBones(aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);

			/// @brief Contains model animations
		Animation m_animations;
			/// @brief Bone name to ID map
		std::map<std::string, int> m_BoneInfoMap;
			/// @brief Stores mesh indexes
		std::vector<int> m_indexes;
			/// @brief Store model directory
		std::string m_directory;
			/// @brief Mesh index to material map
		std::map<int, Material> m_materials;
			/// @brief Stores a models meshes + materials
		std::vector<mesh_material> m_meshes;
	};
}
