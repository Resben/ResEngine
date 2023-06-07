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
// BufferElement
//--------------------------------------------------------------------------------
	BufferElement::BufferElement(BufferElementType type, bool normalize)
		: m_type{ type }, m_normalize{ normalize },
		  m_bytes{ GetTypeSize() }, m_offset{ 0 }
	{

	}

	BufferElementType BufferElement::GetType() const
	{
		return m_type;
	}

	bool BufferElement::GetNormalize() const
	{
		return m_normalize;
	}

	Intptr_t BufferElement::GetOffset() const
	{
		return m_offset;
	}

	unsigned int BufferElement::GetCount() const
	{
		switch (m_type)
		{
		// cases will fall through until they reach the correct value
		case BufferElementType::Bool:
		case BufferElementType::Int:
		case BufferElementType::Float: return 1;
		case BufferElementType::Int2:
		case BufferElementType::Float2: return 2;
		case BufferElementType::Int3:
		case BufferElementType::Float3: return 3;
		case BufferElementType::Int4:
		case BufferElementType::Float4: return 4;
		case BufferElementType::Mat3: return 9;
		case BufferElementType::Mat4: return 16;
		default:
			AE_LOG_FATAL("BufferElement::GetCount::Error -> Invalid BufferElementType");
		}
	}

	Intptr_t BufferElement::GetTypeSize() const
	{
		switch(m_type)
		{
		case BufferElementType::Bool: return sizeof(bool);
		case BufferElementType::Int: return sizeof(int);
		case BufferElementType::Int2: return sizeof(int) * 2;
		case BufferElementType::Int3: return sizeof(int) * 3;
		case BufferElementType::Int4: return sizeof(int) * 4;
		case BufferElementType::Float: return sizeof(float);
		case BufferElementType::Float2: return sizeof(float) * 2;
		case BufferElementType::Float3: return sizeof(float) * 3;
		case BufferElementType::Float4: return sizeof(float) * 4;
		case BufferElementType::Mat3: return sizeof(float) * 9;
		case BufferElementType::Mat4: return sizeof(float) * 16;
		default:
			AE_LOG_FATAL("BufferElement::GetTypeSize::Error -> Invalid BufferElementType");
		}
	}

//--------------------------------------------------------------------------------
// VertexBufferLayout
//--------------------------------------------------------------------------------
	VertexBufferLayout::VertexBufferLayout()
		: m_layout{}, m_stride{ 0 }
	{

	}

	VertexBufferLayout::VertexBufferLayout(std::initializer_list<BufferElement> layout)
		: m_layout{ layout }, m_stride{ 0 }
	{
		CalculateStrideAndOffsets();
	}

	void VertexBufferLayout::AddElement(const BufferElement& element)
	{
		m_layout.push_back(element);
		CalculateStrideAndOffsets();
	}

	const std::vector<BufferElement>& VertexBufferLayout::GetElements() const
	{
		return m_layout;
	}

	Intptr_t VertexBufferLayout::GetStride() const
	{
		return m_stride;
	}

	void VertexBufferLayout::CalculateStrideAndOffsets()
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
		case RenderLibrary::OpenGL:
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
		case RenderLibrary::OpenGL:
			return MakeShared<OpenGLIndexBuffer>();
		default:
			AE_LOG_FATAL("IndexBuffer::Create::RenderLibrary::Error -> None selected");
		}
	}
}
