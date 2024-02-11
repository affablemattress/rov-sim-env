#pragma once

#include "util/containers/tree.hpp"

#include "renderer/texture.hpp"
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace component {
    struct Mesh {
        glm::mat4 transform;
        renderer::Mesh* data;
        renderer::Texture2D* diffuseMap;
        renderer::Texture2D* specularMap;
        renderer::Texture2D* normalMap;
        renderer::shader::Program shader;
    };

    struct Model {
        util::container::tree::root<Mesh> meshTree;
    };
}