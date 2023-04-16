/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Abstract Mesh object
**/
#pragma once

namespace AEngine
{
		/**
		 * @class Mesh
		 * @brief Abstract mesh object
		**/
	class Mesh {
	public:
		/**
		 * @param[in] vertices to construct mesh from
		 * @param[in] indices to generate faces from
		**/
		Mesh(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices);
		/**
		 * @brief Deletes OpenGL data structures
		**/
		~Mesh();
		/**
		 * @brief Binds the Mesh to the rendering API
		 * @retval void
		**/
		void Bind() const;
		/**
		 * @brief Unbinds the Mesh to the rendenring API
		 * @retval void
		**/
		void Unbind() const;
		/**
		 * @brief Returns the total number of indices in index buffer
		**/
		unsigned int GetIndexCount() const;

	private:
		float* m_vertices = nullptr;
		unsigned int* m_indices = nullptr;
		unsigned int m_nVerts, m_nIndices;
		unsigned int m_vbo = 0, m_vao = 0, m_ebo = 0;

		/**
		 * @brief Generates the OpenGL data objects needed
		 * @retval void
		**/
		void Generate();
	};
}
