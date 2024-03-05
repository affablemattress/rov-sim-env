#pragma once 

#include "config.hpp"

#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/texture.hpp"
#include "renderer/material.hpp"
#include "renderer/uniforms/viewModel.hpp"
#include "math.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace renderer {
    struct Part {
        renderer::Material* material;

        renderer::Mesh* mesh;
        math::TransformMatrix* localTransformMatrix;
    };

    void initPart(Part& part, math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, renderer::Material* material);

    void usePart(const renderer::Part& part);

    math::TransformMatrix drawPart(const renderer::Part& part, const math::TransformMatrix* parentTransformMatrix, GLuint u_modelData);
}