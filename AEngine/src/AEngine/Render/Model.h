/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Animation.h"
#include "Animator.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define MAX_BONE_INFLUENCE 4

namespace AEngine
{
		/**
		 * @class Model
		 * @brief Model object, contains material and animations
		**/
	class Model : public Asset
	{
	public:
		using mesh_material = std::pair<SharedPtr<VertexArray>, SharedPtr<Material>>;
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
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, Animator& animator, const TimeStep time);
			/**
			 * @brief Get material for a mesh by index
			 * @param[in] meshIndex Index of mesh
			**/
		const VertexArray* GetMesh(int index) const;

		const Material* GetMaterial(int meshIndex) const;
			/**
			 * @brief Get animation object for model
			 * @return Animation reference
			**/

			/**
			 * @brief Deconstructor
			**/
		~Model();

		Animation* GetAnimation(std::string id);

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
		void LoadTextures(SharedPtr<Material> ae_material, const aiMaterial* ai_material, const aiTextureType ai_type);
			/**
			 * @brief Loads materials from Assimp structures
			 * @param[in] scene Assimp aiScene object
			**/
		SharedPtr<Material> GenerateMaterials(const aiScene* scene, int index);
			/**
			 * @brief Recursively loads meshes from Assimp structures
			 * @param[in] scene Assimp aiScene object
			**/
		void ProcessNode(const aiNode* node, const aiScene* scene);
			/**
			 * @brief Creates a mesh from Assimp structures
			 * @param[in] mesh Assimp aiMesh object
			 * @return Mesh and material pair
			**/
		mesh_material CreateMesh(const aiScene* scene, const aiMesh* mesh);
			/**
			 * @brief Maps assimp bone names to a ID
			 * @param[in] string name of bone
			 * @param[in] bone Assimp aiBone object
			 * @return ID for bone name
			**/
		int NameToID(std::string& name, const aiBone* bone);
			/**
			 * @brief Loads Assimp bone data into vectors
			 * @param[in] mesh Assimp aiMesh object
			 * @param[in] BoneWeights Vector of floats for weight data
			 * @param[in] BoneIDs Vector of ints for ID data
			**/
		void LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);

			/// @brief Bone name to ID map
		std::map<std::string, BoneInfo> m_BoneInfoMap;
			/// @brief Store model directory
		std::string m_directory;
			/// @brief Stores a models meshes + materials
		std::vector<mesh_material> m_meshes;

		std::map<std::string, SharedPtr<Animation>> m_animations;
	};
}