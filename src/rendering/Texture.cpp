/** 
 * Copyright (C) 2018 Tomasz Gałaj
 **/

#include <stb_image.h>

#include "Texture.h"
#include <iostream>
#include <helpers/RootDir.h>

Texture::Texture()
    : use_linear(true), to_id(0)
{
}

Texture::~Texture()
{
    if(to_id != 0)
    {
        glDeleteTextures(1, &to_id);
        to_id = 0;
    }
}

bool Texture::load(const std::string & file_name)
{
    if(file_name.empty())
    {
        return false;
    }

    bool is_loaded = false;
    int width, height, components;
    
    unsigned char* pixels = stbi_load((ROOT_DIR + file_name).c_str(), &width, &height, &components, 4);

    if(pixels != nullptr)
    {
        glGenTextures(1, &to_id);
        glBindTexture(GL_TEXTURE_2D, to_id);
        
        glTexStorage2D(GL_TEXTURE_2D, 2 /* mip map levels */, GL_RGB8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0 /* mip map level */, 0 /* xoffset */, 0 /* yoffset */, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        std::cout << "Could not load file " << file_name << std::endl;
    }

    stbi_image_free(pixels);

    return is_loaded;
}
