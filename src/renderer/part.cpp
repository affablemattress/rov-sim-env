#include "renderer/part.hpp"

namespace renderer {
    void initPart(Part& part, math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, renderer::Material* material) {
        part.material = material;

        part.mesh = mesh; 
        part.localTransformMatrix = localTransformMatrix; 
    }

    void usePart(const renderer::Part& part) {
        renderer::useMaterial(*part.material);
        renderer::useMesh(*part.mesh);
    }

    math::TransformMatrix drawPart(const renderer::Part& part, const math::TransformMatrix* parentTransformMatrix, GLuint u_modelData) {
        math::TransformMatrix partTransformMatrix = glm::inverse(*part.localTransformMatrix) * (*parentTransformMatrix) * (*part.localTransformMatrix);
        renderer::uniformBuffer::ModelData modelData(&partTransformMatrix);

        glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(renderer::uniformBuffer::ModelData), &modelData, GL_DYNAMIC_DRAW);

        renderer::drawMesh(*part.mesh);

        return partTransformMatrix;
    }
}