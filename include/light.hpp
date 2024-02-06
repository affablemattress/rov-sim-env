#pragma once

#include "config.hpp"

#include "glm.hpp"
#include "glad/glad.h"

namespace light {
    struct PointLight {
        glm::vec3 position;

        GLfloat ambientColor[3];
        GLfloat diffuseColor[3];
        GLfloat specularColor[3];

        GLfloat falloffC;
        GLfloat falloffP;
        GLfloat falloffQ;
    };

    struct DirectionalLight {
        glm::vec3 direction;

        GLfloat ambientColor[3];
        GLfloat diffuseColor[3];
        GLfloat specularColor[3];
    };

    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;

        GLfloat diffuseColor[3];
        GLfloat specularColor[3];

        GLfloat coneAlpha;
        GLfloat coneBeta;

        GLfloat falloffC;
        GLfloat falloffP;
        GLfloat falloffQ;
    };

    struct AmbientLight {
        GLfloat intensity;

        GLfloat color[3];
    };
}