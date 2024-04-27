#pragma once
#include "KuroEngineCore/Render/OpenGL/IndexBuffer.h"

namespace KuroEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_array) noexcept;
        VertexArray(VertexArray&& vertex_array) noexcept;

        void add_vertex_buffer(const VertexBuffer& vertex_buffer);
        void set_index_buffer(const IndexBuffer& index_buffer);
        void bind() const;
        static void unbind();

        size_t Get_indices_count() const { return indices_count; };

    private:
        unsigned int vao_id = 0;
        unsigned int vao_elements_count = 0;
        size_t indices_count = 0;
    };

}