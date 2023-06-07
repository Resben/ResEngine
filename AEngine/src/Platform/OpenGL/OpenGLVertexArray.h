/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Render/VertexArray.h"
#include <glad/glad.h>
#include <vector>

namespace AEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
			/**
			 * \copydoc VertexArray::Bind
			*/
		void Bind() const override;
			/**
			 * \copydoc VertexArray::Unbind
			*/
		void Unbind() const override;
			/**
			 * \copydoc VertexArray::AddVertexBuffer
			*/
		void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) override;
			/**
			 * \copydoc VertexArray::SetIndexBuffer
			*/
		void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) override;

			/**
			 * \copydoc VertexArray::GetVertexBuffers
			*/
		const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override;
			/**
			 * \copydoc VertexArray::GetIndexBuffer
			*/
		const SharedPtr<IndexBuffer>& GetIndexBuffer() const override;

	private:
			/**
			 * \brief OpenGL handle to the vertex array
			*/
		GLuint m_id;
			/**
			 * \brief Index for the next vertexAttribute to be added
			*/
		GLuint m_vertexLocationIndex;
			/**
			 * \brief The vertex buffers of the vertex array
			*/
		std::vector<SharedPtr<VertexBuffer>> m_vertexBuffers;
			/**
			 * \brief The index buffer of the vertex array
			*/
		SharedPtr<IndexBuffer> m_indexBuffer;

			/**
			 * \brief Checks if the given buffer element type is an integer type
			 * \param type The buffer element type to check
			 * \retval True The given buffer element type is an integer type
			 * \retval False The given buffer element type is not an integer type
			 * \remark
			 * This was added to optimize the vertex array creation process. \n
			 * Tihs also fixes a bug where the vertex array would be errenous if the buffer element type was integer type.
			*/
		bool IsIntegerType(BufferElementType type) const;
	};
}
