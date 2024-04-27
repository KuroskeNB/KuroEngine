#include "Texture2D.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace KuroEngine
{

    GLuint loadTexture(const char* path) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        return textureID;
    }


    Texture2D::Texture2D(const char* path)
    {
        texture_id = loadTexture(path);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &texture_id);
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept
    {
        glDeleteTextures(1, &texture_id);
        texture_id = texture.texture_id;
        texture_width = texture.texture_width;
        texture_height = texture.texture_height;
        texture.texture_id = 0;
        return *this;
    }

    Texture2D::Texture2D(Texture2D&& texture) noexcept
    {
        texture_id = texture.texture_id;
        texture.texture_id = 0;
    }

    void Texture2D::bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture_id));
    }

}