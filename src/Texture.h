#include <glad.h>
#include "stb_image.h"

#include <iostream>

struct Texture
{
    std::string file;
    unsigned int texture = 0;
    int width{}, height{}, channels{};

    int wrapS     = GL_REPEAT;
    int wrapT     = GL_REPEAT;
    int minFilter = GL_LINEAR_MIPMAP_LINEAR;
    int magFilter = GL_LINEAR;
    int format    = GL_RGB;

    void Init(const char* path)
    {
        file = path;
        Init();
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
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(file.c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void Bind()
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
};