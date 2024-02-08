#pragma once

#pragma pack(1) 
#include "math.hpp"
#include "gameobjects/camera.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace uniform {
    struct PointLightBlock {
        glm::vec4 ambientColor;
        glm::vec4 color;

        glm::vec3 pos;

        GLfloat ambientIntensity;
        GLfloat specularIntensity;

        GLfloat falloffC;
        GLfloat falloffP;
        GLfloat falloffQ;

        PointLightBlock() = default;
        /// @brief Creates a point light source struct that can be directly buffered as a uniform.
        /// @param ambientColor color of the ambient component
        /// @param color color of the specular and diffuse components
        /// @param pos position of the light source
        /// @param ambientIntensity intensity for ambient component
        /// @param specularIntensity intensity for specular component
        /// @param falloff C, P, Q parameters for attenuation
        PointLightBlock(const glm::vec4& ambientColor, const glm::vec4& color, const glm::vec3& pos, 
            GLfloat ambientIntensity, GLfloat specularIntensity,GLfloat falloffC, GLfloat falloffP, GLfloat falloffQ) 
                : ambientColor(ambientColor), color(color), pos(pos),
                  ambientIntensity(ambientIntensity), specularIntensity(specularIntensity), 
                  falloffC(falloffC), falloffP(falloffP), falloffQ(falloffQ) { };
    };

    namespace buffer {
        struct ActiveLights {
            uniform::PointLightBlock pointLights[16];
            GLint pointLightCount;

            ActiveLights(): pointLightCount(0) {};
        };
    }


    /*
    struct DirectionalLight {
        glm::vec3 ambientColor;
        glm::vec3 color;

        glm::vec3 direction;

        GLfloat ambientIntensity;
        GLfloat specularIntensity;
    };

    struct SpotLight {
        glm::vec3 color;

        glm::vec3 pos;
        glm::vec3 dir;

        GLfloat specularIntensity;

        GLfloat coneAlpha;
        GLfloat coneBeta;

        GLfloat falloffC;
        GLfloat falloffP;
        GLfloat falloffQ;
    };

    struct AmbientLight {
        glm::vec3 ambientColor;

        GLfloat ambientIntensity;
    }; 
    */
}

#pragma pack() 