#pragma once

#include <GL/glut.h>

class Texture
{
private:
    int _width;
    int _height;
    int _nrChanells;

    unsigned int _textureId;

public:
    Texture(){};
    Texture(const char *path, bool alpha);

    auto GetTexture() -> unsigned int;
};

#define STB_IMAGE_IMPLEMENTATION
#include "../../deps/include/stb/stb_image.h"

Texture::Texture(const char *path, bool alpha = false)
{
    unsigned int texture;

    auto alphaParameter = alpha ? GL_RGBA : GL_RGB;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(path, &_width, &_height, &_nrChanells, 0);
    stbi_set_flip_vertically_on_load(true);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, alphaParameter, _width, _height, 0, alphaParameter, GL_UNSIGNED_BYTE, data);

        _textureId = texture;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::GetTexture() -> unsigned int
{
    return _textureId;
}

Texture *ARROW;
Texture *BACKGROUND;
Texture *BOX;
Texture *CHEST;
Texture *FIREBALL;
Texture *HEART;
Texture *THREEHEART;
Texture *MAXLIFE;
Texture *KEY;
Texture *MAGMA;
Texture *WALL;
Texture *BOW;
Texture *QUIVER;


