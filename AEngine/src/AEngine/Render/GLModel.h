#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Mesh.h"
#include "Texture.h"

	// Remove from header?
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace AEngine
{
	class GLModel
	{
	public:

		void Delete();
		GLModel(const std::string& path);
		std::shared_ptr<Mesh>& getMesh(int index);
		std::shared_ptr<Texture>& getTextures(int meshIndex);
		int Size();
		~GLModel();

	private:
		void loadTextures(aiMaterial* mat, aiTextureType type);
		void generateMaterials(const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> createMesh(aiMesh* mesh);

		std::vector<unsigned int> indexes;
		std::string directory;
		std::vector<std::shared_ptr<Texture>> m_materials;
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}