#pragma once

#include <glad.h>
#include "stb_image.h"


#include <iostream>
#include <cstring>

struct Texture
{
    std::string file;
    unsigned int texture = 0;
    int width{}, height{}, channels{};
    int textureIndex = GL_TEXTURE0;

    int wrapS     = GL_REPEAT;
    int wrapT     = GL_REPEAT;
    int minFilter = GL_LINEAR_MIPMAP_LINEAR;
    int magFilter = GL_LINEAR;
    int format    = GL_RGBA;

    void Init(const char* path)
    {
        file = path;
        Init();
    }

    ~Texture()
    {
        if (texture) glDeleteTextures(1, &texture);
    }

    void Generate(size_t w, size_t h)
    {
        width = w;
        height = h;
        this->Init();
    }

    void Init()
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        // load and generate the texture
        unsigned char* data = nullptr;
        bool fromFile = !file.empty();

        if(fromFile) {
            stbi_set_flip_vertically_on_load(true);
            data = stbi_load(file.c_str(), &width, &height, &channels, 0);
        } else {
            data = new unsigned char[width * height * 4];
            memset(data, 0, width * height * 4);
        }

        if (channels == 3) format = GL_RGB;
        if (channels == 4) format = GL_RGBA;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "Failed to load texture" << std::endl;
        }
       if(fromFile) stbi_image_free(data);
       else delete[] data;
    }

    void Bind()
    {
        glActiveTexture(textureIndex);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
};
