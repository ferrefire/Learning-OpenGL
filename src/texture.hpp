#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"
#include "errors.hpp"

class Texture
{
    private:
        /* data */
    public:
        unsigned int ID;
        int width;
        int height;
        int nrChannels;

        Texture(const char *path);
        void bindTexture(int textureUnit);
};

#endif