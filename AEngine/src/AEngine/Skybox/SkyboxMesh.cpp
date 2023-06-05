/**
 * \file
 * \author Christien Alden (3411981)
 * \author Geoff Candy
*/
#include "SkyboxMesh.h"
#include <glad/glad.h>

namespace AEngine
{
	AEngine::SkyboxMesh::SkyboxMesh(const float* vertices, Size_t numVerts, Size_t numIndices)
		: m_vao{ 0 }, m_vbo{ 0 }, m_numIndices{ numIndices }
	{
		glGenVertexArrays(1, &m_vao);

		Bind();
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVerts, vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		Unbind();
	}

	SkyboxMesh::~SkyboxMesh()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		m_vao = m_vbo = m_numIndices = 0;
	}

	void SkyboxMesh::Bind() const
	{
		glBindVertexArray(m_vao);
	}

	void SkyboxMesh::Unbind() const
	{
		glBindVertexArray(0);
	}

	Size_t SkyboxMesh::GetNumIndices() const
	{
		return m_numIndices;
	}
}
