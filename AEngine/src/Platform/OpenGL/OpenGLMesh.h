/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include <glad/glad.h>
#include "AEngine/Render/Mesh.h"

namespace AEngine
{
		/**
		 * @class Mesh
		 * @brief OpenGL Mesh object
		**/
	class OpenGLMesh : public Mesh
	{
	public:
			/**
			 * @param[in] vertices to generate vbo
			 * @param[in] nverts in vbo
			 * @param[in] indices to generate ebo
			 * @param[in] nindices in ebo
			 * 
			 * @warning \p nverts must be divisible by 8 and the vertex format must be 
			 * [ 3xfloat for position, 3x float for normal, 2xfloat for texcoords ]
			**/
		OpenGLMesh(GLfloat* vertices, GLuint nverts, GLuint* indices, GLuint nindices);
		virtual ~OpenGLMesh();

			/**
			 * @brief Binds the Mesh to the rendering API
			 * @retval void
			**/
		void Bind() const override;
			/**
			 * @brief Unbinds the Mesh to the rendenring API
			 * @retval void
			**/
		void Unbind() const override;
			/**
			 * @brief Returns the total number of indices in index buffer
			**/
		GLuint GetIndexCount() const override;

	private:
		GLuint m_vao, m_vbo, m_ebo;
		GLfloat* m_vertices;
		GLuint m_nverts;
		GLuint* m_indices;
		GLuint m_nindices;

		void Generate();
	};
}
