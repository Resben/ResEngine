#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Render/VertexArray.h"
#include "AEngine/Resource/Asset.h"
#include "Shader.h"
#include <string>
#include <vector>


namespace AEngine
{
	class HeightMap : public Asset
	{
	public:
		/**
		 * @param[in] data of the heightmap in row-major
		 * @param[in] size of one side of square heightmap
		 * @note x-coordinate should run along the 'rows' of the matrix
		 * @warning \p data must be of length, \p sideLength^2 and
		 * the heightmap is assumed to be square
		**/
		HeightMap::HeightMap(const std::string& ident, const std::string& path);
		HeightMap(const HeightMap& copy);
		~HeightMap();
		const HeightMap& operator=(const HeightMap& rhs);

		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView);
		void Render(
			const Math::mat4& transform,
			const Shader& shader,
			const Math::mat4& projectionView,
			const std::vector<std::string>& textures,
			const std::vector<Math::vec2>& yRanges
		);

		/**
		 * @brief Samples a point on the heightmap
		 * @param[in] xCoord to sample
		 * @param[in] zCoord to sample
		 * @return Value at point
		 * @throws invalid_argument if params are out of bounds
		**/
		float samplePoint(Size_t xCoord, Size_t zCoord) const;

		/**
		 * @brief Samples a point on the heightmap with the original data
		 * @param[in] xCoord to sample
		 * @param[in] zCoord to sample
		 * @return Value at point in range [0, 1]
		 * @throws invalid_argument if params are out of bounds
		**/
		float samplePointRaw(Size_t xCoord, Size_t zCoord) const;

		/**
		 * @brief Generates an OpenGL mesh to pass to renderer
		 * @retval Mesh of HeightMap
		**/
		SharedPtr<VertexArray> CreateMesh();

		VertexArray* GetMesh() const;

		void NormaliseColour(unsigned char* imgData);

		Size_t getSideLength() const;

		static SharedPtr<HeightMap> Create(const std::string& ident, const std::string& fname);

	private:
		float* m_data;
		float m_min, m_max, m_range;
		int m_channels;

		SharedPtr<VertexArray> m_mesh;

		Size_t m_size;
		Size_t m_sideLength;

		void normalise();
	};
}
