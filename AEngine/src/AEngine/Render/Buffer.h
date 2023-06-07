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

	class BufferData
	{
	public:
		BufferData(BufferDataType type, bool normalize = false);
		BufferDataType GetType() const;
		bool GetNormalize() const;
		Intptr_t GetOffset() const;
		unsigned int GetCount() const;

	private:
		BufferDataType m_type;
		bool m_normalize;
		Intptr_t m_bytes;
		Intptr_t m_offset;

		static Intptr_t GetTypeSize(BufferDataType type);
		friend class VertexBufferLayout;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		VertexBufferLayout(std::initializer_list<BufferData> layout);
		const std::vector<BufferData>& GetElements() const;
		Intptr_t GetStride() const;

	private:
		std::vector<BufferData> m_layout;
		Intptr_t m_stride;
		void CalculateStride();
	};

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
			 * \details
			 * The usage parameter is used to determine how the buffer will be used.
			 * - StaticDraw: The data will be uploaded once and drawn many times
			 * - DynamicDraw: The data will be uploaded many times and drawn many times
			 * - StreamDraw: The data will be uploaded once and used a few times
			*/
		virtual void SetData(void* data, Intptr_t bytes, BufferUsage usage = BufferUsage::StaticDraw) = 0;
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
		VertexBufferLayout m_layout;
	};

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
			 * \details
			 * The usage parameter is used to determine how the buffer will be used.
			 * - StaticDraw: The data will be uploaded once and drawn many times
			 * - DynamicDraw: The data will be uploaded many times and drawn many times
			 * - StreamDraw: The data will be uploaded once and used a few times
			*/
		virtual void SetData(Uint32* data, Intptr_t count, BufferUsage usage = BufferUsage::StaticDraw) = 0;
			/**
			 * \brief Creates a new index buffer
			 * \return Shared pointer to the index buffer
			*/
		static SharedPtr<IndexBuffer> Create();
	};
}
