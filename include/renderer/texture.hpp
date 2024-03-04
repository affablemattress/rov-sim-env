#pragma once

#include "config.hpp"

#include "glad/glad.h"

#include <string>

namespace component {
}

namespace renderer {
    using TextureBuffer = GLuint;

    enum TextureType {
        k_diffuse,
        k_specular,
        k_normal,
        k_emissive,
        k_occlusion,
        k_none
    };

    struct d_Texture2D {
        TextureType type;
        std::string filepath;

        int x;
        int y;
        int numChannels;
        unsigned char* buffer; 
    };

    struct Texture2D {
        TextureBuffer bufferID;

        TextureType type;
        std::string filepath;
    };



    d_Texture2D* loadTexture2DFromFile(std::string_view filepath, TextureType type);
    void freeTexture2D(d_Texture2D* data);

    void pushTexture2D(renderer::Texture2D& texture, d_Texture2D* data);

    void useTexture2D(renderer::Texture2D& texture, GLuint imageUnitIndex);
}