#pragma once
#include <string>
#include "AEngine/Core/Types.h"
#include "AEngine/Render/Mesh.h"
#include "AEngine/Resource/Asset.h"

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

		/**
		 * @brief Samples a point on the heightmap
		 * @param[in] xCoord to sample
		 * @param[in] zCoord to sample
		 * @return Value at point
		 * @throws invalid_argument if params are out of bounds
		**/
		float samplePoint(size_t xCoord, size_t zCoord) const;

		/**
		 * @brief Samples a point on the heightmap with the original data
		 * @param[in] xCoord to sample
		 * @param[in] zCoord to sample
		 * @return Value at point in range [0, 1]
		 * @throws invalid_argument if params are out of bounds
		**/
		float samplePointRaw(size_t xCoord, size_t zCoord) const;

		/**
		 * @brief Generates an OpenGL mesh to pass to renderer
		 * @retval Mesh of HeightMap
		**/
		SharedPtr<Mesh> CreateMesh();

		SharedPtr<Mesh> GetMesh() const;

		void NormaliseColour(unsigned char* imgData);

		size_t getSideLength() const;

		static SharedPtr<HeightMap> Create(const std::string& ident, const std::string& fname);

	private:
		float* m_data;
		float m_min, m_max, m_range;
		int m_channels;

		SharedPtr<Mesh> m_mesh;

		size_t m_size;
		size_t m_sideLength;

		bool copyData(const float* const& data);
		void normalise();
	};
}
