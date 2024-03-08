#pragma once

#include "util/containers/tree.hpp"

#include "renderer/texture.hpp"
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/part.hpp"
#include "renderer/model.hpp"
#include "math.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"

#include <vector>

namespace renderer {
    using Model = util::container::Tree<Part*>;
    using ModelNode = util::container::Tree<Part*>;

    void loadAndPushAssimpModel(Assimp::Importer& aiImporter, Model& model, std::string_view modelPath,
                                Shader* shader, std::vector<Texture2D>& textures, std::vector<Material>& materials,
                                std::vector<Mesh>& meshes, std::vector<math::TransformMatrix>& localTransforms, std::vector<Part>& parts);

    void drawModel(const Model* model, const math::TransformMatrix* modelTransformMatrix, GLuint u_modelData);
}