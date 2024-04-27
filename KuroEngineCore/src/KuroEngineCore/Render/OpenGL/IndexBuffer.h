#pragma once
#include "VertexBuffer.h"

namespace KuroEngine
{

	class IndexBuffer
	{
	public:

		IndexBuffer(const void* data, const size_t size,const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
		~IndexBuffer();

		IndexBuffer() = delete;
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer& vertexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& vertexBuffer) noexcept;

		void bind() const;
		static void unbind();

		size_t GetCount() const { return buffer_count; };
	private:
		unsigned int buffer_id = 0;
		size_t buffer_count = 0;
	};

}