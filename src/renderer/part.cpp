#include "renderer/part.hpp"

namespace renderer {
    Part::Part(math::TransformMatrix* localTransformMatrix, renderer::Mesh* mesh, renderer::Material* material) : 
        localTransformMatrix(localTransformMatrix), 
        mesh(mesh), 
        material(material) { }

    void usePart(const renderer::Part& part) {
        renderer::useMesh(*part.mesh);
        renderer::useMaterial(*part.material);
    }

    void drawPart(const renderer::Part& part) {
        glDrawElements(GL_TRIANGLES, part.mesh->numElements, GL_UNSIGNED_INT, (void *)0);
    }
}