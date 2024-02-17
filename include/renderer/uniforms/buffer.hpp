#pragma once

#include "math.hpp"
#include "gameobjects/camera.hpp"
#include "renderer/uniforms/light.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#pragma pack(1)
namespace renderer {
    namespace uniform {
        namespace buffer {
            struct ActiveLights {
                renderer::uniform::block::PointLight pointLights[16];
                GLint pointLightCount;

                ActiveLights(): pointLightCount(0) {};
            };
        }
    }
}
#pragma pack() 
