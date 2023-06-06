/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Render/Buffer.h"
#include <glad/glad.h>

namespace AEngine
{
	class OpenGLBuffer : public Buffer
	{
	public:
		OpenGLBuffer(BufferType type);
		virtual ~OpenGLBuffer();
			/**
			 * \copydoc Buffer::Bind
			*/
		virtual void Bind() const override;
			/**
			 * \copydoc Buffer::Unbind
			*/
		virtual void Unbind() const override;
			/**
			 * \copydoc Buffer::Size
			*/
		virtual Intptr_t Size() const override;
			/**
			 * \copydoc Buffer::SetData
			*/
		virtual void SetData(void* data, Intptr_t bytes, BufferUsage usage = BufferUsage::StaticDraw) override;
			/**
			 * \copydoc Buffer::SetSubData
			*/
		virtual void SetSubData(void* data, Intptr_t bytes, Intptr_t offset = 0) override;
			/**
			 * \copydoc Buffer::GetType
			*/
		virtual BufferType GetType() const override;

	private:
			/**
			 * \brief The OpenGL handle to the vertex buffer
			*/
		GLuint m_id;
			/**
			 * \brief The size of the vertex buffer in bytes
			*/
		GLsizeiptr m_size;
			/**
			 * \brief The type of buffer for use in engine
			*/
		BufferType m_type;
			/**
			 * \brief The type of opengl buffer
			*/
		GLenum m_glType;
	};
}
