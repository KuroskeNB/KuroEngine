#include "KuroEngineCore/Render/OpenGL/VertexBuffer.h"
#include "KuroEngineCore/Log.h"
#include <iostream>
#include <glad/glad.h>


namespace KuroEngine
{
	constexpr unsigned int shader_data_type_to_components_count(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
			return 1;

		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;

		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;

		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;
		}

		LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
		return 0;
	}

	constexpr size_t shader_data_type_size(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return sizeof(GLfloat) * shader_data_type_to_components_count(type);

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return sizeof(GLint) * shader_data_type_to_components_count(type);
		}

		LOG_ERROR("shader_data_type_size: unknown ShaderDataType!");
		return 0;
	}


	constexpr unsigned int shader_data_type_to_component_type(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return GL_FLOAT;

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		}
	}
	
	BufferElement::BufferElement(const ShaderDataType newtype)
		:type(newtype), component_type(shader_data_type_to_component_type(newtype))
		, components_count(shader_data_type_to_components_count(newtype))
		, size(shader_data_type_size(newtype))
		, offset(0)
	{
	}

	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
		case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
		}
		LOG_ERROR("Unknown VertexBuffer usage");
		return GL_STREAM_DRAW;
	}

	VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout newbuffer_layout, const EUsage usage)
		:buffer_layout(std::move(newbuffer_layout))
	{
		glGenBuffers(1, &buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));

		for (const BufferElement& element : buffer_layout.get_elements())
		{
			LOG_INFO("buffer offsets {0}", element.offset);
		}
	}
	

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1,&buffer_id);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer& vertexBuffer) noexcept
	{
		buffer_id = vertexBuffer.buffer_id;
		vertexBuffer.buffer_id = 0;
		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
		:buffer_layout(vertexBuffer.buffer_layout)
	{
		buffer_id = vertexBuffer.buffer_id;
		vertexBuffer.buffer_id = 0;
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	}

	void VertexBuffer::unbind()
	{
	   glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: layout_elements(elements)
	{
		layout_stride = 0;
		size_t offset = 0;
		for (auto& element : layout_elements)
		{
			element.offset=offset;
			offset += element.size;
			layout_stride += element.size;
		}
	}
}