/**
 * @file
 * @author Christien Alden (34119981)
**/
#include "OpenGLMesh.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	OpenGLMesh::OpenGLMesh(GLfloat* vertices, GLuint nverts, GLuint* indices, GLuint nindices, GLint* boneIDs, GLfloat* boneWeights, GLint numBoneInfluence)
		: m_vertices(vertices), m_indices(indices), m_nverts(nverts), m_nindices(nindices), m_boneIDs(boneIDs), m_boneWeights(boneWeights), m_boneInfluence(numBoneInfluence)
	{
		Generate();
	}

	OpenGLMesh::OpenGLMesh(GLfloat* vertices, GLuint nverts, GLuint* indices, GLuint nindices)
		: m_vertices(vertices), m_indices(indices), m_nverts(nverts), m_nindices(nindices), m_boneIDs(nullptr), m_boneWeights(nullptr), m_boneInfluence(0)
	{
		Generate();
	}

	OpenGLMesh::~OpenGLMesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		m_vao = m_vbo = m_ebo = 0;
	}

	void OpenGLMesh::Bind() const
	{
		glBindVertexArray(m_vao);
	}

	void OpenGLMesh::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLMesh::Generate()
	{
		// generate opengl objects
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		Bind();

		// vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_nverts * sizeof(GL_FLOAT), &m_vertices[0], GL_STATIC_DRAW);

		// vao - position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);

		// vao - normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));

		// vao - texcoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(float)));

		if(m_boneIDs != nullptr || m_boneWeights != nullptr)
		{
			glGenBuffers(1, &m_vbo2);
			glGenBuffers(1, &m_vbo3);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
			glBufferData(GL_ARRAY_BUFFER, m_nverts * m_boneInfluence * sizeof(GL_INT), &m_boneIDs[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(3);
			glVertexAttribIPointer(3, m_boneInfluence, GL_INT, m_boneInfluence * sizeof(GL_INT), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo3);
			glBufferData(GL_ARRAY_BUFFER, m_nverts * m_boneInfluence * sizeof(GL_FLOAT), &m_boneWeights[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, m_boneInfluence, GL_FLOAT, GL_FALSE, m_boneInfluence * sizeof(GL_FLOAT), (void*)0);
		}

		// ebo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nindices * sizeof(GL_UNSIGNED_INT), &m_indices[0], GL_STATIC_DRAW);

		Unbind();
	}

	GLuint OpenGLMesh::GetIndexCount() const
	{
		return m_nindices;
	}
}
