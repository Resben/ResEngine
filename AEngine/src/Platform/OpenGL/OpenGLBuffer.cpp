/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "OpenGLBuffer.h"
#include <array>

namespace
{
		/// \todo Rework this to be less prone to errors
	static constexpr std::array<GLenum, 2> g_glBufferType = {
		GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER
	};

	static constexpr std::array<GLenum, 3> g_glBufferUsage = {
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW,
		GL_STREAM_DRAW
	};
}

namespace AEngine
{
//--------------------------------------------------------------------------------
// VertexBuffer
//--------------------------------------------------------------------------------
	OpenGLBuffer::OpenGLBuffer(BufferType type)
		: m_id{ 0 }, m_size{ 0 }, m_type{ type },
		  m_glType{ g_glBufferType[static_cast<int>(type)] }
	{
		glGenBuffers(1, &m_id);
	}

	OpenGLBuffer::~OpenGLBuffer()
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
		m_size = 0;
	}

	void OpenGLBuffer::Bind() const
	{
		glBindBuffer(m_glType, m_id);
	}

	void OpenGLBuffer::Unbind() const
	{
		glBindBuffer(m_glType, 0);
	}

	Intptr_t OpenGLBuffer::Size() const
	{
		return static_cast<Intptr_t>(m_size);
	}

	void OpenGLBuffer::SetData(void* data, Intptr_t bytes, BufferUsage usage)
	{
		m_size = static_cast<GLsizeiptr>(bytes);

		Bind();
		GLenum glUsage = g_glBufferUsage[static_cast<int>(usage)];
		glBufferData(m_glType, m_size, data, glUsage);
		Unbind();
	}

	void OpenGLBuffer::SetSubData(void* data, Intptr_t bytes, Intptr_t offset)
	{
		Bind();
		glBufferSubData(m_glType, offset, static_cast<GLsizeiptr>(bytes), data);
		Unbind();
	}

	BufferType OpenGLBuffer::GetType() const
	{
		return m_type;
	}
}
