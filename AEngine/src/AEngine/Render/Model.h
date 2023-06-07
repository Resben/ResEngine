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

	struct Material
	{
		std::string DiffuseTexture;
		std::string SpecularTexture;
	};

	class Model : public Asset
	{
	public:
		using mesh_material = std::pair<SharedPtr<Mesh>, int>;
		Model(const std::string& ident, const std::string& path);
		void Clear();
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, const TimeStep time);
		const SharedPtr<Mesh>& GetMesh(int index) const;
		const Material* GetMaterial(int meshIndex) const;
		Animation& GetAnimation();
		int Size() const;
		~Model();

		// for rendering
		std::vector<mesh_material>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<mesh_material>::const_iterator end() const { return m_meshes.end(); }

		static SharedPtr<Model> Create(const std::string& ident, const std::string& fname);

	private:

		std::string LoadTextures(aiMaterial* mat, aiTextureType type);
		void GenerateMaterials(const aiScene* scene);
		void ProcessNode(aiNode* node, const aiScene* scene);
		mesh_material CreateMesh(aiMesh* mesh);
		int NameToID(std::string& name, aiBone* bone);
		void LoadMeshBones(aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs);

		Animation m_animations;
		std::map<std::string, int> m_BoneInfoMap;
		std::vector<int> m_indexes;
		std::string m_directory;
		std::map<int, Material> m_materials;
		std::vector<mesh_material> m_meshes;
	};
}
