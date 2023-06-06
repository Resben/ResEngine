#include "Model.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/AssetManager.h"
#include "RenderCommand.h"

#ifdef AE_RENDER_OPENGL
#include "Platform/OpenGL/OpenGLMesh.h"
#endif

namespace AEngine
{
	SharedPtr<Model> Model::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<Model>(ident, fname);
	}

	using mesh_material = std::pair<SharedPtr<Mesh>, int>;

	Model::Model(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		m_directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

		GenerateMaterials(scene);

		AE_LOG_TRACE("Model::Constructor::Success -> {}", path);
	}

	Model::~Model()
	{
		Clear();
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			m_meshes.push_back(
				CreateMesh(scene->mMeshes[node->mMeshes[i]])
			);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	/*
	 * NOTE:
	 * Is it necessary to copy data
	 * Possibly needed for physics
	*/
	mesh_material Model::CreateMesh(aiMesh* mesh)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			if (mesh->HasNormals())
			{
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
			if (mesh->HasTextureCoords(0))
			{
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int y = 0; y < face.mNumIndices; y++)
			{
				indices.push_back(face.mIndices[y]);
			}
		}

		// generate structures
		m_indexes.push_back(mesh->mMaterialIndex);
		return std::make_pair(Mesh::Create(vertices.data(), static_cast<unsigned int>(vertices.size()), indices.data(), static_cast<unsigned int>(indices.size())), mesh->mMaterialIndex);
	}

	void Model::Clear()
	{
		m_materials.clear();
		m_meshes.clear();
		AE_LOG_DEBUG("Model::Clear");
	}

	void Model::Render(const Math::mat4& transform, const Shader &shader, const Math::mat4 &projectionView) const
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

		for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			/// @todo Make this work with other material types...
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);

			tex->Bind();
			mesh.Bind();

			// draw
			RenderCommand::DrawIndexed(GraphicsEnum::DrawTriangles, mesh.GetIndexCount(), 0);

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}

	std::string Model::LoadTextures(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = str.C_Str();
			Size_t last = filename.find_last_of("\\");

			if (last != std::string::npos)
				filename = filename.substr(last + 1);

			std::string path = m_directory + "/" + filename;

			AssetManager<Texture>::Instance().Load(path);

			return filename;
		}
	}

	void Model::GenerateMaterials(const aiScene* scene)
	{
		for (unsigned int i = 0; i < m_indexes.size(); i++)
		{
			if (m_indexes[i] >= 0)
			{
				Material material;
				aiMaterial* mat = scene->mMaterials[m_indexes[i]];
				material.DiffuseTexture = LoadTextures(mat, aiTextureType_DIFFUSE);
				//material.SpecularTexture = LoadTextures(mat, aiTextureType_SPECULAR);
				m_materials.emplace(std::make_pair(m_indexes[i], material));
			}
		}
	}

	const SharedPtr<Mesh>& Model::GetMesh(int index) const
	{
		if (index > m_meshes.size())
		{
			AE_LOG_FATAL("Model::GetMesh::Out of Bounds");
		}

		return m_meshes[index].first;
	}

	const Material* Model::GetMaterial(int meshIndex) const
	{
		std::map<int, Material>::const_iterator it;
		it = m_materials.find(meshIndex);
		if (it != m_materials.end())
			return &it->second;
		else
			return nullptr;
	}

	int Model::Size() const
	{
		return static_cast<int>(m_meshes.size());
	}
}