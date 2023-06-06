/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "Types.h"

namespace AEngine
{
	class Buffer
	{
	public:
		virtual ~Buffer() = default;
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
			 * \brief Returns the type of the buffer
			 * \return Type of the buffer
			 * \see BufferType
			*/
		virtual BufferType GetType() const = 0;

			/**
			 * \brief Creates a new vertex buffer
			 * \return Shared pointer to the vertex buffer
			*/
		static SharedPtr<Buffer> Create(BufferType type);
	};
}
