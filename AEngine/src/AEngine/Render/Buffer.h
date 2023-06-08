/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "Types.h"
#include <initializer_list>
#include <string>
#include <vector>

namespace AEngine
{
	class VertexBufferLayout;

		/**
		 * \class BufferElement
		 * \brief Rendering API agnostic buffer element
		 * \details
		 * The buffer element is used to describe the layout of the vertex buffer
		*/
	class BufferElement
	{
	public:
			/**
			 * \param[in] type The type of the buffer element
			 * \param[in] normalize Whether the data should be normalized
			*/
		BufferElement(BufferElementType type, bool normalize = false);
			/**
			 *	\brief Returns the number of atomic data in this buffer element
			 * \return Number of atomic data in this buffer element
			*/
		unsigned int GetCount() const;
			/**
			 * \brief Returns the type of the buffer element
			 * \return Type of the buffer element
			*/
		BufferElementType GetType() const;
			/**
			 * \brief Returns the precision of the buffer element
			 * \return Precision of the buffer element
			*/
		BufferElementPrecision GetPrecision() const;
			/**
			 * \brief Returns whether the data should be normalized
			 * \return Whether the data should be normalized
			*/
		bool GetNormalize() const;
			/**
			 * \brief Returns the offset to the start of this buffer element in bytes
			 * \return Offset to the start of this buffer element in bytes
			*/
		Intptr_t GetOffset() const;

	private:
			/**
			 * \brief The type of the buffer element
			*/
		BufferElementType m_type;
			/**
			 * \brief Whether the data should be normalized
			*/
		bool m_normalize;
			/**
			 * \brief The offset to the start of this buffer element in bytes
			*/
		Intptr_t m_offset;
			/**
			 * \brief The precision of the buffer element
			*/
		BufferElementPrecision m_precision;
			/**
			 * \brief The size of the atomic type of this buffer element in bytes
			*/
		Intptr_t m_atomicSize;
			/**
			 * \brief The number of atomic data in this buffer element
			*/
		unsigned int m_count;

		friend class VertexBufferLayout;
	};

		/**
		 * \class VertexBufferLayout
		 * \brief Rendering API agnostic vertex buffer layout
		 * \details
		 * The vertex buffer layout is used to describe the layout of the vertex buffer. \n
		 * This composes of a list of buffer elements.
		 * \warning
		 * The order of the buffer elements in the vertex buffer layout must match the order of the buffer elements in the vertex buffer.
		*/
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
			/**
			 * \param[in] layout List of buffer elements
			*/
		VertexBufferLayout(std::initializer_list<BufferElement> layout);
			/**
			 * \brief Add a buffer element to the layout
			 * \param[in] element Buffer element to be added
			*/
		void AddElement(const BufferElement& element);
			/**
			 * \brief Returns the list of buffer elements
			 * \return List of buffer elements
			*/
		const std::vector<BufferElement>& GetElements() const;
			/**
			 * \brief Returns the stride of the vertex buffer layout
			 * \return Stride of the vertex buffer layout
			*/
		Intptr_t GetStride() const;

	private:
			/**
			 * \brief List of buffer elements
			*/
		std::vector<BufferElement> m_layout;
			/**
			 * \brief Stride of the vertex buffer layout
			*/
		Intptr_t m_stride;
			/**
			 * \brief Calculate the stride and offsets of the vertex buffer layout
			 * \note This is called automatically when a buffer element is added
			*/
		void CalculateStrideAndOffsets();
	};

		/**
		 * \class VertexBuffer
		 * \brief Rendering API agnostic vertex buffer
		*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
			/**
			 * \brief Bind the vertex buffer
			*/
		virtual void Bind() const = 0;
			/**
			 * \brief Unbind the vertex buffer
			*/
		virtual void Unbind() const = 0;
			/**
			 * \brief Returns the size of the vertex buffer in bytes
			 * \return Size of the vertex buffer in bytes
			*/
		virtual Intptr_t Size() const = 0;
			/**
			 * \brief Set the data of the vertex buffer
			 * \param[in] data Pointer to the data to be uploaded
			 * \param[in] bytes Size of the data in bytes
			 * \param[in] usage Usage of the buffer
			*/
		virtual void SetData(void* data, Intptr_t bytes, BufferUsage usage) = 0;
			/**
			 * \brief Sets a portion of the vertex buffer data
			 * \param[in] data Pointer to the data to be uploaded
			 * \param[in] bytes Size of the data in bytes
			 * \param[in] offset Offset in bytes to start uploading the data
			*/
		virtual void SetSubData(void* data, Intptr_t bytes, Intptr_t offset = 0) = 0;
			/**
			 * \brief Sets the layout of the vertex buffer
			 * \param[in] layout Layout of the vertex buffer
			*/
		void SetLayout(const VertexBufferLayout& layout);
			/**
			 * \brief Returns the layout of the vertex buffer
			 * \return Layout of the vertex buffer
			*/
		const VertexBufferLayout& GetLayout() const;
			/**
			 * \brief Creates a new vertex buffer
			 * \return Shared pointer to the vertex buffer
			*/
		static SharedPtr<VertexBuffer> Create();

	private:
			/**
			 * \brief Layout of the vertex buffer
			*/
		VertexBufferLayout m_layout;
	};

		/**
		 * \class IndexBuffer
		 * \brief Rendering API agnostic index buffer
		*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
			/**
			 * \brief Bind the index buffer
			*/
		virtual void Bind() const = 0;
			/**
			 * \brief Unbind the index buffer
			*/
		virtual void Unbind() const = 0;
			/**
			 * \brief Returns the size of the index buffer in bytes
			 * \return Size of the index buffer in bytes
			*/
		virtual Intptr_t GetCount() const = 0;
			/**
			 * \brief Set the data of the index buffer
			 * \param[in] data Pointer to the data to be uploaded
			 * \param[in] count the number of elements in the data
			 * \param[in] usage Usage of the buffer
			*/
		virtual void SetData(Uint32* data, Intptr_t count, BufferUsage usage) = 0;
			/**
			 * \brief Creates a new index buffer
			 * \return Shared pointer to the index buffer
			*/
		static SharedPtr<IndexBuffer> Create();
	};
}
