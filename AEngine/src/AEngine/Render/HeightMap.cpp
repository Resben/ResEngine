#include <stb/stb_image.h>
#include <stdexcept>

#include "Heightmap.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Logger.h"
#include "RenderCommand.h"
#include "Texture.h"
#include "AEngine/Resource/AssetManager.h"

namespace AEngine
{

	SharedPtr<HeightMap> HeightMap::Create(const std::string& ident, const std::string& fname)
	{
		return std::make_shared<HeightMap>(ident, fname);
	}

	HeightMap::HeightMap(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{
		AE_LOG_DEBUG("HeightMap::Constructor");

		int width = 0;
		int height = 0;
		m_channels = 0;
		unsigned char* imgData = nullptr;

		stbi_set_flip_vertically_on_load(true);

		imgData = stbi_load(path.c_str(), &width, &height, &m_channels, 0);

		if (width != height)
		{
			AE_LOG_FATAL("HeightMap::Constructor -> width and height are not 1:1");
		}

		m_sideLength = width;
		m_size = m_sideLength * m_sideLength;
		m_data = new float[m_size];

		NormaliseColour(imgData);
		normalise();
		m_mesh = CreateMesh();
	}

	HeightMap::~HeightMap()
	{
		AE_LOG_DEBUG("HeightMap::Destructor");
		if (m_data)
		{
			delete[] m_data;
		}
	}

	void HeightMap::Render(const Math::mat4 &transform, const Shader &shader, const Math::mat4& projectionView)
	{
		Render(transform, shader, projectionView, {}, {});
	}

	void HeightMap::Render(const Math::mat4 &transform,  const Shader &shader, const Math::mat4& projectionView, const std::vector<std::string> &textures, const std::vector<Math::vec2> &yRanges)
	{
		Size_t tsize = textures.size();

		shader.Bind();
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

			//probably merge later
		for (Size_t y = 0; y < tsize; y++)
		{
			std::string textureUniform = "u_textures[" + std::to_string(y) + "]";
			std::string rangeUniform = "u_yRanges[" + std::to_string(y) + "]";
			shader.SetUniformInteger(textureUniform, static_cast<int>(y));
			shader.SetUniformFloat2(rangeUniform, yRanges[y]);
		}

		shader.SetUniformInteger("u_numTextures", static_cast<int>(tsize));
		for (Size_t i = 0; i < tsize; i++)
		{
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(textures[i]);
			tex->Bind(static_cast<int>(i));
		}

		// bind mesh
		Mesh& mesh = *GetMesh();
		mesh.Bind();

		RenderCommand::DrawIndexed(PrimitiveDraw::Triangles, mesh.GetIndexCount(), 0);

		for (Size_t i = 0; i < tsize; i++)
		{
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(textures[i]);
			tex->Unbind();
		}

		mesh.Unbind();
		shader.Unbind();
	}

	void HeightMap::NormaliseColour(unsigned char *imgData)
	{
		for (unsigned int i = 0; i < m_size; i++)
		{
			m_data[i] = static_cast<float>(
				imgData[i * m_channels] +
				imgData[i * m_channels + 1] +
				imgData[i * m_channels + 2] / 3.0f
				);
		}

		stbi_image_free(imgData);
	}

	void HeightMap::normalise()
	{
		m_min = m_data[0];
		m_max = m_data[0];

		// find min/max of data
		for (Size_t i = 0; i < m_size; ++i)
		{
			if (m_data[i] > m_max)
			{
				m_max = m_data[i];
			}
			else if (m_data[i] < m_min)
			{
				m_min = m_data[i];
			}
		}

		// scale all values
		m_range = m_max - m_min;
		for (Size_t i = 0; i < m_size; ++i)
		{
			m_data[i] = static_cast<float>((m_data[i] - m_min) / m_range);
		}
	}

	float HeightMap::samplePoint(Size_t xCoord, Size_t zCoord) const
	{
		// return negative value on invalid arguments
		if ((xCoord >= m_sideLength) || (zCoord >= m_sideLength))
		{
			throw std::invalid_argument("xCoord/zCoord out of bounds");
		}

		return m_data[(zCoord * m_sideLength) + xCoord];
	}

	float HeightMap::samplePointRaw(Size_t xCoord, Size_t zCoord) const
	{
		// return negative value on invalid arguments
		if ((xCoord >= m_sideLength) || (zCoord >= m_sideLength))
		{
			throw std::invalid_argument("xCoord/zCoord out of bounds");
		}

		return (m_data[(zCoord * m_sideLength) + xCoord] * m_range) + m_min;
	}

	SharedPtr<Mesh> HeightMap::CreateMesh()
	{
		// 2 triangles per quad, with 3 indices per triangle
		Size_t numQuads = m_size - (2 * m_sideLength) + 1;
		Size_t numTriangles = numQuads * 2;
		unsigned int indexArraySize = static_cast<unsigned int>(numTriangles * 3);

		// generate index array
		unsigned int* elementArray = new unsigned int[indexArraySize];

		// using right-hand coordinates
		unsigned int ei = 0;
		for (Size_t xi = 0; xi < m_sideLength - 1; ++xi)
		{
			for (Size_t zi = 0; zi < m_sideLength; ++zi)
			{
				if (zi < m_sideLength - 1)
				{
					// add top-left txiangle
					elementArray[ei++] = static_cast<unsigned int>((zi * m_sideLength) + xi);
					elementArray[ei++] = static_cast<unsigned int>((zi * m_sideLength) + xi + 1);
					elementArray[ei++] = static_cast<unsigned int>(((zi + 1) * m_sideLength) + xi);
				}

				// add bottom-xight txiangle
				if (zi > 0)
				{
					elementArray[ei++] = static_cast<unsigned int>((zi * m_sideLength) + xi);
					elementArray[ei++] = static_cast<unsigned int>(((zi - 1) * m_sideLength) + xi + 1);
					elementArray[ei++] = static_cast<unsigned int>((zi * m_sideLength) + xi + 1);
				}
			}
		}

		// generate vertex array
		// 3 for vertex position
		// 2 for texture coordinates
		unsigned int vertexArraySize = static_cast<unsigned int>(m_size * (3 + 3 + 2));
		float* vertexArray = new float[vertexArraySize];
		unsigned int vi = 0;
		for (Size_t xi = 0; xi < m_sideLength; ++xi)
		{
			for (Size_t zi = 0; zi < m_sideLength; ++zi)
			{
				// position of vertex
				vertexArray[vi++] = xi / static_cast<float>(m_sideLength - 1);
				vertexArray[vi++] = samplePoint(xi, zi);
				vertexArray[vi++] = zi / static_cast<float>(m_sideLength - 1);

				float point = samplePoint(xi, zi);
				Math::vec3 normal(0.0f);
					// If not an edge piece
				if (xi > 0 && xi < m_sideLength - 1 && zi > 0 && zi < m_sideLength - 1) {
						// Get neighbouring vector
					Math::vec3 left(-1.0f, samplePoint(xi - 1, zi) - point, 0.0f);
					Math::vec3 right(1.0f, samplePoint(xi + 1, zi) - point, 0.0f);
					Math::vec3 down(0.0f, samplePoint(xi, zi - 1) - point, -1.0f);
					Math::vec3 up(0.0f, samplePoint(xi, zi + 1) - point, 1.0f);
						// Calculate a normal
					normal = Math::normalize(Math::cross(left, up) + Math::cross(up, right) + Math::cross(right, down) + Math::cross(down, left));
				}

				vertexArray[vi++] = normal.x;
				vertexArray[vi++] = normal.y;
				vertexArray[vi++] = normal.z;

				// @todo look into a better way to set UVs
				vertexArray[vi++] = xi / static_cast<float>(m_sideLength - 1) * 100.0f;
				vertexArray[vi++] = zi / static_cast<float>(m_sideLength - 1) * 100.0f;
			}
		}

		return Mesh::Create(vertexArray, vertexArraySize, elementArray, indexArraySize);
	}

	SharedPtr<Mesh> HeightMap::GetMesh() const
	{
		return m_mesh;
	}

	Size_t HeightMap::getSideLength() const
	{
		return m_sideLength;
	}
}
