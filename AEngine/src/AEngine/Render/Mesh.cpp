/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Abstract Mesh object
**/
#include <glad/glad.h>
#include "Mesh.h"

namespace AEngine
{
	Mesh::Mesh(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices)
		: m_vertices(vertices), m_indices(indices), m_nVerts(nverts), m_nIndices(nindices)
	{
		Generate();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		m_vao = m_vbo = 0;
	}

	void Mesh::Bind() const
	{
		glBindVertexArray(m_vao);
	}

	void Mesh::Unbind() const
	{
		glBindVertexArray(0);
	}

	void Mesh::Generate()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_nVerts * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

		// ebo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndices * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		// normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		Unbind();
	}

	unsigned int Mesh::GetIndexCount() const
	{
		return m_nIndices;
	}
}
