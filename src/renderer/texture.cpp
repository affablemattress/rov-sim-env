#include "renderer/texture.hpp"

#include "app.hpp"

#include "stb_image.h"
#include "spdlog/spdlog.h"

#include <string>
#include <filesystem>

namespace renderer {
    d_Texture2D* loadTexture2DFromFile(std::string_view filepath, TextureType type) {
        if(!std::filesystem::exists(filepath)) {
            spdlog::error("File not found while attempting to load Texture2D at: {0}", filepath.data());
            app::lifetime::killAll(1);
        }
        else {
            spdlog::info("Loading Texture2D at: {0}", filepath.data());
        }

        d_Texture2D* data = new d_Texture2D();

        data->filepath = filepath;

        data->type = type; 
        data->buffer = stbi_load(filepath.data(), &data->x, &data->y, &data->numChannels, 3);

        return data;
    }

    void freeTexture2D(d_Texture2D* data) {
        stbi_image_free(data->buffer);
        delete data;
    }

    void pushTexture2D(renderer::Texture2D& texture, d_Texture2D* data) {
        glGenTextures(1, &texture.bufferID);
        glBindTexture(GL_TEXTURE_2D, texture.bufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data->x, data->y, 0, GL_RGB, GL_UNSIGNED_BYTE, data->buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);

        texture.type = data->type;
        texture.filepath = data->filepath;
    }

    void useTexture2D(renderer::Texture2D& texture, GLuint imageUnitIndex) {
        glActiveTexture(imageUnitIndex);
        glBindTexture(GL_TEXTURE_2D, texture.bufferID);
    }
}