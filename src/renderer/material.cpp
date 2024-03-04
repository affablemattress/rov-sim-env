#include "renderer/material.hpp"

namespace renderer {
    Material::Material(renderer::Texture2D* diffuseMap, renderer::Texture2D* specularMap, renderer::Texture2D* normalMap, renderer::Texture2D* occlusionMap, renderer::Texture2D* emissiveMap, renderer::Shader* shader) :
        diffuseMap(diffuseMap),
        specularMap(specularMap),
        normalMap(normalMap),
        occlusionMap(occlusionMap),
        emissiveMap(emissiveMap),
        shader(shader) { }

    void useMaterial(const Material& material) {
        useTexture2D(*material.diffuseMap, GL_TEXTURE0);
        useTexture2D(*material.specularMap, GL_TEXTURE0 + 1);/*
        useTexture2D(*material.normalMap, GL_TEXTURE0 + 2);
        useTexture2D(*material.emissiveMap, GL_TEXTURE0 + 3);
        useTexture2D(*material.occlusionMap, GL_TEXTURE0 + 4);*/

        useShader(*material.shader);
    }
}