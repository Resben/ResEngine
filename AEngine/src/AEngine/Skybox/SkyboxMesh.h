/**
 * \file
 * \author Christien Alden (3411981)
 * \author Geoff Candy
*/
#pragma once
#include "AEngine/Core/Types.h"

namespace AEngine
{
	class SkyboxMesh
	{
	public:
			/**
			 * \param[in] vertices The vertices of the skybox.
			 * \param[in] numVerts The number of vertices.
			 * \param[in] numIndices The number of indices.
			*/
		SkyboxMesh(const float* vertices, Size_t numVerts, Size_t numIndices);
		~SkyboxMesh();
			/**
			 * \brief Binds the skybox mesh.
			*/
		void Bind() const;
			/**
			 * \brief Unbinds the skybox mesh.
			*/
		void Unbind() const;
			/**
			 * \brief Gets the number of indices.
			 * \return The number of indices.
			*/
		Size_t GetNumIndices() const;

	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		Size_t m_numIndices;
	};
}
