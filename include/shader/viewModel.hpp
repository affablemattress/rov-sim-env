#pragma once
#pragma pack(1)

#include "math.hpp"
#include "camera.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace uniform {
    struct CameraData {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 pos;

        CameraData() = delete;
        /// @brief Generates the view and projection matrices in place.
        /// @param camera 
        CameraData(const camera::Object& camera) {
            viewMatrix = camera::buildViewMatrix(camera);
            projectionMatrix = camera::buildProjectionMatrix(camera);
            pos = { camera.position.x, camera.position.y, camera.position.z};
        }
    };

    struct ModelData {
        glm::mat4 modelMatrix;
        glm::mat4 normalMatrix;

        ModelData() = delete;
        /// @brief Generates the modelMatrix and normalMatrix matrices in place.
        /// @param position float[3]
        /// @param rotation float[3]
        /// @param scale float[3]
        ModelData(const float* position, const float* rotation, const float* scale) {
            modelMatrix = math::buildModelMatrix(position, rotation, scale);
            normalMatrix = math::buildNormalMatrixFromModelMatrix(modelMatrix);
        }
    };
}