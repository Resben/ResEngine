/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Render/Buffer.h"
#include <glad/glad.h>

namespace AEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer();
			/**
			 * \copydoc VertexBuffer::Bind
			*/
		virtual void Bind() const override;
			/**
			 * \copydoc VertexBuffer::Unbind
			*/
		virtual void Unbind() const override;
			/**
			 * \copydoc VertexBuffer::Size
			*/
		virtual Intptr_t Size() const override;
			/**
			 * \copydoc VertexBuffer::GetCount
			*/
		virtual Intptr_t GetCount() const override;
			/**
			 * \copydoc VertexBuffer::SetData
			*/
		virtual void SetData(void* data, Intptr_t bytes, BufferUsage usage) override;
			/**
			 * \copydoc VertexBuffer::SetSubData
			*/
		virtual void SetSubData(void* data, Intptr_t bytes, Intptr_t offset = 0) override;

	private:
			/**
			 * \brief The OpenGL handle to the vertex buffer
			*/
		GLuint m_id;
			/**
			 * \brief The size of the vertex buffer in bytes
			*/
		GLsizeiptr m_size;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer();
		virtual ~OpenGLIndexBuffer();
			/**
			 * \copydoc IndexBuffer::Bind
			*/
		virtual void Bind() const override;
			/**
			 * \copydoc IndexBuffer::Unbind
			*/
		virtual void Unbind() const override;
			/**
			 * \copydoc IndexBuffer::GetCount
			*/
		virtual Intptr_t GetCount() const override;
			/**
			 * \copydoc IndexBuffer::SetData
			*/
		virtual void SetData(Uint32* data, Intptr_t count, BufferUsage usage) override;

	private:
			/**
			 * \brief The OpenGL handle to the index buffer
			*/
		GLuint m_id;
			/**
			 * \brief The number of indices in the index buffer
			*/
		GLsizeiptr m_count;
	};
}