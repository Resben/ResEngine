/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "Buffer.h"
#include <vector>

namespace AEngine
{
		/**
		 * \class VertexArray
		 * \brief Rendering API agnostic vertex array
		*/
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
			/**
			 * \brief Binds the vertex array
			*/
		virtual void Bind() const = 0;
			/**
			 * \brief Unbinds the vertex array
			*/
		virtual void Unbind() const = 0;
			/**
			 * \brief Adds a vertex buffer to the vertex array
			 * \param[in] vertexBuffer The vertex buffer to add
			*/
		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) = 0;
			/**
			 * \brief Sets the index buffer of the vertex array
			 * \param[in] indexBuffer The index buffer to set
			*/
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) = 0;
			/**
			 * \brief Gets the vertex buffers of the vertex array
			 * \return The vertex buffers of the vertex array
			*/
		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
			/**
			 * \brief Gets the index buffer of the vertex array
			 * \return The index buffer of the vertex array
			*/
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;
			/**
			 * \brief Creates a vertex array
			 * \return The created vertex array
			*/
		static SharedPtr<VertexArray> Create();
	};
}
