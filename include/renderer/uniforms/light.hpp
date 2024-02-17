#pragma once

#include "math.hpp"
#include "gameobjects/camera.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#pragma pack(1)
namespace renderer {
    namespace uniform {
        namespace block {
            struct PointLight {
                glm::vec4 ambientColor;
                glm::vec4 color;

                glm::vec3 pos;

                GLfloat ambientIntensity;
                GLfloat specularIntensity;

                GLfloat falloffC;
                GLfloat falloffP;
                GLfloat falloffQ;

                PointLight() = default;

                PointLight(const glm::vec4& ambientColor, const glm::vec4& color, const glm::vec3& pos, 
                        GLfloat ambientIntensity, GLfloat specularIntensity,GLfloat falloffC, GLfloat falloffP, GLfloat falloffQ) : 
                    ambientColor(ambientColor), color(color), pos(pos),
                    ambientIntensity(ambientIntensity), specularIntensity(specularIntensity), 
                    falloffC(falloffC), falloffP(falloffP), falloffQ(falloffQ) { };
            };
        }
    }
}
#pragma pack() 

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
