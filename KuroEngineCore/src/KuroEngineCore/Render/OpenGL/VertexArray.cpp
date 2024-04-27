#include "VertexArray.h"
#include "KuroEngineCore/Log.h"
#include <glad/glad.h>

namespace KuroEngine
{

   VertexArray::VertexArray()
	{
	   glGenVertexArrays(1, &vao_id);
	}

   VertexArray::~VertexArray()
   {
	   glDeleteVertexArrays(1, &vao_id);
   }

   VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept
   {
	   vao_id = vertex_array.vao_id;
	   vao_elements_count = vertex_array.vao_elements_count;
	   vertex_array.vao_id = 0;
	   vertex_array.vao_elements_count = 0;

	   return *this;
   }

   VertexArray::VertexArray(VertexArray&& vertex_array) noexcept
   {
	   vao_id = vertex_array.vao_id;
	   vao_elements_count = vertex_array.vao_elements_count;
	   vertex_array.vao_id = 0;
	   vertex_array.vao_elements_count = 0;
   }

   void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer)
   {
	   bind();
	   vertex_buffer.bind();
	   for (const BufferElement& element : vertex_buffer.GetBufferLayout().get_elements())
	   {
		   glEnableVertexAttribArray(vao_elements_count);
		   glVertexAttribPointer
		   (vao_elements_count,
			   static_cast<GLint>(element.components_count),
			   element.component_type,
			   GL_FALSE,
			   static_cast<GLsizei>(vertex_buffer.GetBufferLayout().get_stride()),
			   reinterpret_cast<const void*>(element.offset));
		   vao_elements_count++;
	   }
   }

   void VertexArray::set_index_buffer(const IndexBuffer& index_buffer)
   {
	   bind();
	   index_buffer.bind();
	   indices_count = index_buffer.GetCount();
   }

   void VertexArray::bind() const
   {
	   glBindVertexArray(vao_id);
   }

   void VertexArray::unbind()
   {
	   glBindVertexArray(0);
   }


}
