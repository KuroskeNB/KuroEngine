#include "IndexBuffer.h"
#include <glad/glad.h>

namespace KuroEngine
{
	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
		case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
		}
		return GL_STREAM_DRAW;
	}

	IndexBuffer::IndexBuffer(const void* data, const size_t size, const VertexBuffer::EUsage usage)
		: buffer_count(size)
	{
		glGenBuffers(1, &buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_count*sizeof(GLuint), data, usage_to_GLenum(usage));
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &buffer_id);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer& vertexBuffer) noexcept
	{
		buffer_id = vertexBuffer.buffer_id;
		vertexBuffer.buffer_id = 0;

		buffer_count = vertexBuffer.buffer_count;
		vertexBuffer.buffer_count = 0;
		return *this;
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& vertexBuffer) noexcept
	{
		buffer_id = vertexBuffer.buffer_id;
		vertexBuffer.buffer_id = 0;

		buffer_count = vertexBuffer.buffer_count;
		vertexBuffer.buffer_count = 0;
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
	}

	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}




}