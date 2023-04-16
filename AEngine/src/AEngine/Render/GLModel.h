#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Mesh.h"
#include "Texture.h"

	// Remove from header?
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace AEngine
{

	struct Material
	{
		std::string DiffuseTexture;
		std::string SpecularTexture;
	};

	class GLModel
	{
	public:

		void Clear();
		GLModel(const std::string& path);
		std::shared_ptr<Mesh>& GetMesh(int index);
		Material* GetMaterial(int meshIndex);
		int Size();
		~GLModel();

	private:
		std::string& LoadTextures(aiMaterial* mat, aiTextureType type);
		void GenerateMaterials(const aiScene* scene);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> CreateMesh(aiMesh* mesh);

		std::vector<unsigned int> m_indexes;
		std::string m_directory;
		std::map<unsigned int, Material> m_materials;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}