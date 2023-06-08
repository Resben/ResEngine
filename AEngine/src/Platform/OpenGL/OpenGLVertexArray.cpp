/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "OpenGLVertexArray.h"
#include "AEngine/Core/Logger.h"

namespace
{
	static constexpr GLenum g_glDataTypes[] = {
		GL_BYTE, GL_BYTE, GL_BYTE, GL_BYTE,                                       // bytes
		GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,   // ubytes
		GL_INT, GL_INT, GL_INT, GL_INT,                                           // ints
		GL_UNSIGNED_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,       // uints
		GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT,                                   // floats
		GL_FLOAT, GL_FLOAT                                                        // mat3, mat4
	};
}

namespace AEngine
{
	OpenGLVertexArray::OpenGLVertexArray()
		: m_id{ 0 }, m_vertexLocationIndex{ 0 }, m_vertexBuffers{}, m_indexBuffer{ nullptr }
	{
		glGenVertexArrays(1, &m_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
		m_vertexBuffers.clear();
		m_indexBuffer = nullptr;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
	{
		// bind vertex array and vertex buffer
		Bind();
		vertexBuffer->Bind();

		// get layout and data
		const VertexBufferLayout& layout = vertexBuffer->GetLayout();
		const std::vector<BufferElement> layoutData = layout.GetElements();
		const Size_t size = layoutData.size();
		if (size == 0)
		{
			AE_LOG_ERROR("Vertex buffer has no layout!");
			Unbind();
			return;
		}

		for (Size_t i = 0; i < size; ++i)
		{
			const BufferElement& data = layoutData[i];
			glEnableVertexAttribArray(m_vertexLocationIndex);
			if (data.GetPrecision() == BufferElementPrecision::Integer)
			{
				glVertexAttribIPointer(
					m_vertexLocationIndex,
					data.GetCount(),
					g_glDataTypes[static_cast<int>(data.GetType())],
					static_cast<GLsizei>(layout.GetStride()),
					(const void*) data.GetOffset()
				);
			}
			else
			{
				glVertexAttribPointer(
					m_vertexLocationIndex,
					data.GetCount(),
					g_glDataTypes[static_cast<int>(data.GetType())],
					data.GetNormalize(),
					static_cast<GLsizei>(layout.GetStride()),
					(const void*) data.GetOffset()
				);
			}

			++m_vertexLocationIndex;
		}

		Unbind();
		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		Unbind();

		m_indexBuffer = indexBuffer;
	}

	const std::vector<SharedPtr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const SharedPtr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
}
