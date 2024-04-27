#pragma once
#include <vector>

namespace KuroEngine
{
	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
	};

	struct BufferElement
	{
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType newtype);
	};

	class BufferLayout
	{
	public:
		BufferLayout() = delete;
		BufferLayout(std::initializer_list<BufferElement> elements);
		
		const std::vector<BufferElement>& get_elements() const { return layout_elements; }
		size_t get_stride() const { return layout_stride; }

	private:
		std::vector<BufferElement> layout_elements;
		size_t layout_stride = 0;
	};

	class VertexBuffer
	{
	public:
		enum class EUsage
		{
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout newbuffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer() = delete;
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		void bind() const;
		static void unbind();
		const BufferLayout& GetBufferLayout() const{ return buffer_layout; };
	private:
		unsigned int buffer_id = 0;
		BufferLayout buffer_layout;
	};

}