/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "OpenGLVertexArray.h"
#include "AEngine/Core/Logger.h"

namespace
{
	static constexpr std::array<GLenum, 11> g_glDataTypes = {
		GL_BOOL,
		GL_INT, GL_INT, GL_INT, GL_INT,
		GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT,
		GL_FLOAT, GL_FLOAT
	};
}

namespace AEngine
{
	OpenGLVertexArray::OpenGLVertexArray()
		: m_id{ 0 }, m_vertexBuffers{}, m_indexBuffer{ nullptr }
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
		const std::vector<BufferData> layoutData = layout.GetElements();
		const Size_t size = layoutData.size();
		if (size == 0)
		{
			AE_LOG_ERROR("Vertex buffer has no layout!");
			Unbind();
			return;
		}

		for (Size_t i = 0; i < size; ++i)
		{
			const BufferData& data = layoutData[i];
			glEnableVertexAttribArray(static_cast<GLuint>(m_vertexLocationIndex));
			glVertexAttribPointer(
				static_cast<GLuint>(m_vertexLocationIndex),
				data.GetCount(),
				g_glDataTypes[static_cast<int>(data.GetType())],
				data.GetNormalize(),
				static_cast<GLsizei>(layout.GetStride()),
				(const void*) data.GetOffset()
			);

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
