#pragma once 

#include "config.hpp"

#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/texture.hpp"
#include "math.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace renderer {
    struct Part {
        math::TransformMatrix* localTransformMatrix;
        renderer::Mesh* mesh;
        renderer::Texture2D* diffuseMap;
        renderer::Texture2D* specularMap;
        renderer::Texture2D* normalMap;
        renderer::shader::Program* shader;

        Part(math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, renderer::Texture2D* diffuseMap, renderer::Texture2D* specularMap, renderer::Texture2D* normalMap, renderer::shader::Program* shader);
    };

    void attachPart(const renderer::Part& part);
    void drawPart(const renderer::Part& part);
}