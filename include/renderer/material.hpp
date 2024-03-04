#pragma once 

#include "config.hpp"

#include "renderer/texture.hpp"
#include "renderer/shader.hpp"

namespace renderer {
    struct Material {
        renderer::Texture2D* diffuseMap;
        renderer::Texture2D* specularMap;
        renderer::Texture2D* normalMap;
        renderer::Texture2D* emissiveMap;
        renderer::Texture2D* occlusionMap;

        renderer::Shader* shader;

        Material() = delete;
        Material(renderer::Texture2D* diffuseMap, renderer::Texture2D* specularMap, renderer::Texture2D* normalMap, renderer::Texture2D* emissiveMap, renderer::Texture2D* occlusionMap, renderer::Shader* shader);
    };

    void useMaterial(const Material& material);
}