#pragma once

#include "util/containers/tree.hpp"

#include "renderer/texture.hpp"
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/part.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace component {

    struct Model {
        util::container::tree::root<Mesh> meshTree;
    };
}