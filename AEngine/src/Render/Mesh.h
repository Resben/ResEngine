/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Abstract Mesh object
**/
#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace AEngine
{
		/**
		 * @struct Vertex
		 * @brief Holds data corresponding to one vertex of a Mesh
		**/
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

		/**
		 * @class Mesh
		 * @brief Abstract mesh object
		 * @todo Remove OpenGL specific components
		**/
	class Mesh {
	public:
			/**
			 * @param[in] vertices to construct mesh from
			 * @param[in] indices to generate faces from
			**/
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
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
			 * @brief Returns the underlying vertex data
			 * @retval std::vector<Vertex>&
			**/
		const std::vector<Vertex>& GetVertices() const;

			/**
			 * @brief Returns the underlying index structure
			 * @retval std::vector<unsigned int>&
			**/
		const std::vector<unsigned int>& GetIndices() const;

	private:
			/// Vertices that make up the Mesh
		std::vector<Vertex> m_vertices;
			/// Indices to make up faces of Mesh
		std::vector<unsigned int> m_indices;

			// OpenGL specific handles
		unsigned int m_vbo, m_vao;

			/**
			 * @brief Generates the OpenGL data objects needed
			 * @retval void
			**/
		void Generate();
	};
}
