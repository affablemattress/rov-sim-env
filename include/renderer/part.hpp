#pragma once 

#include "config.hpp"

#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/texture.hpp"
#include "renderer/material.hpp"
#include "math.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace renderer {
    struct Part {
        math::TransformMatrix* localTransformMatrix;

        renderer::Mesh* mesh;

        renderer::Material* material;

        Part() = delete;
        Part(math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, renderer::Material* material);
    };

    void usePart(const renderer::Part& part);

    void drawPart(const renderer::Part& part);
}