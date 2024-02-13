#pragma once 

#include "config.hpp"

#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace renderer {
    struct Part {
        glm::mat4 transform;
        renderer::Mesh* data;
        renderer::Texture2D* diffuseMap;
        renderer::Texture2D* specularMap;
        renderer::Texture2D* normalMap;
        renderer::shader::Program* shader;
    };
}