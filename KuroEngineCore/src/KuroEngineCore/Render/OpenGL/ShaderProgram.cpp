#include "KuroEngineCore/Render/OpenGL/ShaderProgram.h"
#include "KuroEngineCore/Log.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>



namespace KuroEngine {

    bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id)
    {
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &source, nullptr);
        glCompileShader(shader_id);

        GLint success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            return false;
        }
        return true;
    }

	ShaderProgram::ShaderProgram(const char* newvertex_shader, const char* newfragment_shader)
        : vertex_shader(newvertex_shader), fragment_shader(newfragment_shader)
	{
        GLuint vertex_shader_id = 0;
        if (!create_shader(newvertex_shader, GL_VERTEX_SHADER, vertex_shader_id))
        {
            LOG_CRITICAL("VERTEX SHADER: compile-time error!");
            glDeleteShader(vertex_shader_id);
            return;
        }

        GLuint fragment_shader_id = 0;
        if (!create_shader(newfragment_shader, GL_FRAGMENT_SHADER, fragment_shader_id))
        {
            LOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }
        
        shader_program_id = glCreateProgram();

        glAttachShader(shader_program_id, vertex_shader_id);
        glAttachShader(shader_program_id, fragment_shader_id);
        glLinkProgram(shader_program_id);

        GLint success;
        glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

        if (success == GL_FALSE)
        {
            GLchar info_log[1024];
            glGetProgramInfoLog(shader_program_id, 1024, nullptr, info_log);
            LOG_CRITICAL("SHADER PROGRAM: Link-time error:\n{0}", info_log);
            glDeleteProgram(shader_program_id);
            shader_program_id = 0;
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }
        else
        {
            bIsCompiled = true;
        }

        glDetachShader(shader_program_id, vertex_shader_id);
        glDetachShader(shader_program_id, fragment_shader_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
	}

    void ShaderProgram::bind() const
    {
        glUseProgram(shader_program_id);
    }

    void ShaderProgram::unbind()
    {
        glUseProgram(0);
    }

    void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(shader_program_id,name),1,GL_FALSE,glm::value_ptr(matrix));
    }

    void ShaderProgram::SetImage(const char* name, const int& texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture));

        glUseProgram(shader_program_id);
        glUniform1i(glGetUniformLocation(shader_program_id, name),0);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaders)
    {
        glDeleteProgram(shader_program_id);
        shader_program_id = shaders.shader_program_id;
        bIsCompiled = shaders.bIsCompiled;

        shaders.shader_program_id = 0;
        shaders.bIsCompiled = false;
        return *this;   
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaders)
    {
        shader_program_id = shaders.shader_program_id;
        bIsCompiled = shaders.bIsCompiled;

        shaders.shader_program_id = 0;
        shaders.bIsCompiled = false;
    }
}