/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Buffer.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace
{
	constexpr unsigned int g_aeBufferElementTypeCounts[] = {
		1, 2, 3, 4,   // bytes
		1, 2, 3, 4,   // ubytes
		1, 2, 3, 4,   // ints
		1, 2, 3, 4,   // uints
		1, 2, 3, 4,   // floats
		9, 16          // mat3, mat4
	};

	constexpr AEngine::BufferElementPrecision g_aeBufferElementTypePrecisions[] = {
#define BEP AEngine::BufferElementPrecision
		BEP::Integer, BEP::Integer, BEP::Integer, BEP::Integer,   // bytes
		BEP::Integer, BEP::Integer, BEP::Integer, BEP::Integer,   // ubytes
		BEP::Integer, BEP::Integer, BEP::Integer, BEP::Integer,   // ints
		BEP::Integer, BEP::Integer, BEP::Integer, BEP::Integer,   // uints
		BEP::Float, BEP::Float, BEP::Float, BEP::Float,           // floats
		BEP::Float, BEP::Float,                                   // mat3, mat4
#undef BEP
	};

	constexpr AEngine::Intptr_t g_aeBufferElementSizes[] = {
#define SO(x) sizeof(x)
		SO(char), SO(char), SO(char), SO(char),                                       // bytes
		SO(unsigned char), SO(unsigned char), SO(unsigned char), SO(unsigned char),   // ubytes
		SO(int), SO(int), SO(int), SO(int),                                           // ints
		SO(unsigned int), SO(unsigned int), SO(unsigned int), SO(unsigned int),       // uints
		SO(float), SO(float), SO(float), SO(float),                                   // floats
		SO(float), SO(float),                                                         // mat3, mat4
#undef SO
	};
}

namespace AEngine
{
//--------------------------------------------------------------------------------
// BufferElement
//--------------------------------------------------------------------------------
	BufferElement::BufferElement(BufferElementType type, bool normalize)
		: m_type{ type }, m_normalize{ normalize }, m_offset{ 0 },
		  m_precision{ g_aeBufferElementTypePrecisions[static_cast<int>(type)] },
		  m_atomicSize{ g_aeBufferElementSizes[static_cast<int>(type)] },
		  m_count{ g_aeBufferElementTypeCounts[static_cast<int>(type)] }
	{

	}

	unsigned int BufferElement::GetCount() const
	{
		return m_count;
	}

	BufferElementType BufferElement::GetType() const
	{
		return m_type;
	}

	BufferElementPrecision BufferElement::GetPrecision() const
	{
		return m_precision;
	}

	bool BufferElement::GetNormalize() const
	{
		return m_normalize;
	}

	Intptr_t BufferElement::GetOffset() const
	{
		return m_offset;
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
			m_stride += data.m_count * data.m_atomicSize;
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
