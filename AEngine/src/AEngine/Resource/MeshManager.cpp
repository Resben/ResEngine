/**
 * @file
 * @author Ben Hawkins
 * @author Christien Alden (34119981)
 * @date 12/09/2022
*/
#include "../Core/Logger.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

#include "../Math/Math.hpp"
#include "MeshManager.h"

namespace fs = std::filesystem;

namespace AEngine
{
	MeshManager* MeshManager::s_instance = nullptr;

	MeshManager* MeshManager::Instance()
	{
		if (!s_instance)
		{
			s_instance = new MeshManager;
		}

		return s_instance;
	}

	void MeshManager::Clear()
	{
		AE_LOG_DEBUG("MeshManager::Clear");
		m_meshes.clear();
	}

	void MeshManager::LoadModelDirectory(const std::string& dir)
	{
		for (const auto& entry : fs::directory_iterator(dir + '/'))
		{
			if (is_regular_file(entry))
			{
				LoadModel(fs::path(entry).string());
			}
		}
	}

	std::shared_ptr<Mesh> MeshManager::LoadModel(const std::string& filename)
	{
		std::ifstream srcFile(filename);
		if (!srcFile)
		{
			AE_LOG_ERROR("MeshManager::Load::Failed -> {}", filename);
			exit(1);
		}

		std::string meshName;
		std::vector<Math::vec3> vertex;
		std::vector<Math::vec3> normal;
		std::vector<Math::vec2> texcoord;
		std::vector<unsigned int> face;

		std::string line;
		while (srcFile.good())
		{
			std::getline(srcFile, line, '\n');
			std::stringstream ss(line);
			std::string prefix;
			ss >> prefix;

			if (prefix == "o")
			{
				ss >> meshName;
			}
			else if (prefix == "v")
			{
				Math::vec3 vert;
				int i = 0;
				while (i < 3 && ss >> vert[i])
				{
					i++;
				}
				vertex.push_back(Math::vec4(vert, 1));
			}
			else if (prefix == "vt")
			{
				Math::vec2 tex;
				int i = 0;
				while (i < 2 && ss >> tex[i])
				{
					i++;
				}
				texcoord.push_back(tex);
			}
			else if (prefix == "vn")
			{
				Math::vec3 norm;
				int i = 0;
				while (i < 3 && ss >> norm[i])
				{
					i++;
				}
				normal.push_back(Math::vec4(norm, 1));
			}
			else if (prefix == "f")
			{
				unsigned int val;
				while (ss >> val)
				{
					face.push_back(val - 1);
					if (ss.peek() == '/')
					{
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ')
					{
						ss.ignore(1, ' ');
					}
				}
			}
		}

		srcFile.close();

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		size_t size = face.size();
		for (int i = 0; i + 2 < size; i = i + 3)
		{
			Vertex vertice;

			unsigned int vi = face[i];
			unsigned int ti = face[i + 1];
			unsigned int ni = face[i + 2];

			vertice.Position = vertex[vi];
			vertice.TexCoords = texcoord[ti];
			vertice.Normal = normal[ni];

			vertices.push_back(vertice);
			indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
		}

		m_meshes.emplace(std::make_pair(
			meshName, std::make_shared<Mesh>(vertices, indices))
		);

		AE_LOG_TRACE("MeshManager::Load::Success -> {}", meshName);
		return m_meshes[meshName];
	}

	Mesh* MeshManager::GetMesh(const std::string& meshName)
	{
		std::map<std::string, std::shared_ptr<Mesh>>::iterator it;
		it = m_meshes.find(meshName);
		if (it != m_meshes.end())
			return it->second.get();
		else
			return nullptr;
	}
}
