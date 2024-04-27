#pragma once

#include <glm/mat4x4.hpp>

namespace KuroEngine {

	class ShaderProgram
	{
	public:
		ShaderProgram(const char* newvertex_shader, const char* newfragment_shader);
        ShaderProgram(ShaderProgram&& shaders);
        ShaderProgram& operator=(ShaderProgram&& shaders);
        ~ShaderProgram() {};

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void bind() const;
        static void unbind();
        bool isCompiled() const { return bIsCompiled; }

        void SetMatrix4(const char* name, const glm::mat4& matrix);
        void SetImage(const char* name, const int& texture);

        unsigned int shaders_program=0;
    private:
        bool bIsCompiled = false;
        unsigned int shader_program_id = 0;


        const char* vertex_shader;
        const char* fragment_shader;
	};
}