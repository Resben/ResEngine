/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Buffer.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// BufferData
//--------------------------------------------------------------------------------
	BufferData::BufferData(BufferDataType type, bool normalize)
		: m_type{ type }, m_normalize{ normalize },
		  m_bytes{ GetTypeSize(type) }, m_offset{ 0 }
	{

	}

	BufferDataType BufferData::GetType() const
	{
		return m_type;
	}

	bool BufferData::GetNormalize() const
	{
		return m_normalize;
	}

	Intptr_t BufferData::GetOffset() const
	{
		return m_offset;
	}

	unsigned int BufferData::GetCount() const
	{
		switch (m_type)
		{
		// cases will fall through until they reach the correct value
		case BufferDataType::Bool:
		case BufferDataType::Int:
		case BufferDataType::Float: return 1;
		case BufferDataType::Int2:
		case BufferDataType::Float2: return 2;
		case BufferDataType::Int3:
		case BufferDataType::Float3: return 3;
		case BufferDataType::Int4:
		case BufferDataType::Float4: return 4;
		case BufferDataType::Mat3: return 9;
		case BufferDataType::Mat4: return 16;
		default:
			AE_LOG_FATAL("BufferData::GetCount::Error -> Invalid BufferDataType");
		}
	}

	Intptr_t BufferData::GetTypeSize(BufferDataType type)
	{
		switch(type)
		{
		case BufferDataType::Bool: return sizeof(bool);
		case BufferDataType::Int: return sizeof(int);
		case BufferDataType::Int2: return sizeof(int) * 2;
		case BufferDataType::Int3: return sizeof(int) * 3;
		case BufferDataType::Int4: return sizeof(int) * 4;
		case BufferDataType::Float: return sizeof(float);
		case BufferDataType::Float2: return sizeof(float) * 2;
		case BufferDataType::Float3: return sizeof(float) * 3;
		case BufferDataType::Float4: return sizeof(float) * 4;
		case BufferDataType::Mat3: return sizeof(float) * 9;
		case BufferDataType::Mat4: return sizeof(float) * 16;
		default:
			AE_LOG_FATAL("BufferData::GetTypeSize::Error -> Invalid BufferDataType");
		}
	}

//--------------------------------------------------------------------------------
// VertexBufferLayout
//--------------------------------------------------------------------------------
	VertexBufferLayout::VertexBufferLayout()
		: m_layout{}, m_stride{ 0 }
	{

	}

	VertexBufferLayout::VertexBufferLayout(std::initializer_list<BufferData> layout)
		: m_layout{ layout }, m_stride{ 0 }
	{
		CalculateStride();
	}

	const std::vector<BufferData>& VertexBufferLayout::GetElements() const
	{
		return m_layout;
	}

	Intptr_t VertexBufferLayout::GetStride() const
	{
		return m_stride;
	}

	void VertexBufferLayout::CalculateStride()
	{
		// for each element in layout, add the size of the element to the stride
		// and set the offset of the element to the current stride
		for (auto& data : m_layout)
		{
			data.m_offset = m_stride;
			m_stride += data.m_bytes;
		}
	}

//--------------------------------------------------------------------------------
// VertexBuffer
//--------------------------------------------------------------------------------
	SharedPtr<VertexBuffer> VertexBuffer::Create()
	{
		switch (RenderCommand::GetLibrary())
		{
		case GraphicsLibrary::OpenGL:
			return MakeShared<OpenGLVertexBuffer>();
		default:
			AE_LOG_FATAL("VertexBuffer::Create::RenderLibrary::Error -> None selected");
		}
	}

	void VertexBuffer::SetLayout(const VertexBufferLayout& layout)
	{
		m_layout = layout;
	}

	const VertexBufferLayout& VertexBuffer::GetLayout() const
	{
		return m_layout;
	}

//--------------------------------------------------------------------------------
// IndexBuffer
//--------------------------------------------------------------------------------
	SharedPtr<IndexBuffer> IndexBuffer::Create()
	{
		switch (RenderCommand::GetLibrary())
		{
		case GraphicsLibrary::OpenGL:
			return MakeShared<OpenGLIndexBuffer>();
		default:
			AE_LOG_FATAL("IndexBuffer::Create::RenderLibrary::Error -> None selected");
		}
	}
}
