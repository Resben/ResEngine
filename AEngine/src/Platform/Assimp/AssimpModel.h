/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"

#include "AEngine/Render/Model.h"

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
	class AssimpModel : public Model
	{
	public:
		using mesh_material = std::pair<SharedPtr<VertexArray>, SharedPtr<Material>>;

		AssimpModel(const std::string& ident, const std::string& path);

		void Clear();
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, Animator& animator, const TimeStep time);

		virtual const VertexArray* GetMesh(int index) const override;

		const Material* GetMaterial(int meshIndex) const;


		virtual ~AssimpModel();

		virtual Animation* GetAnimation(std::string id) override;

		std::vector<mesh_material>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<mesh_material>::const_iterator end() const { return m_meshes.end(); }

	private:

		void LoadTextures(SharedPtr<Material> ae_material, const aiMaterial* ai_material, const aiTextureType ai_type);

		SharedPtr<Material> GenerateMaterials(const aiScene* scene, int index);

		void ProcessNode(const aiNode* node, const aiScene* scene);

		mesh_material CreateMesh(const aiScene* scene, const aiMesh* mesh);

		int NameToID(std::string& name, const aiBone* bone);

		void LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);

		std::map<std::string, BoneInfo> m_BoneInfoMap;

		std::string m_directory;

		std::vector<mesh_material> m_meshes;

		std::map<std::string, SharedPtr<Animation>> m_animations;
	};
}