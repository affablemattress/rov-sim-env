#include "gameobjects/light.hpp"

namespace gameobject {
    namespace light {
        Point::Point(const glm::vec4& ambientColor, const glm::vec4& color, const glm::vec3& pos, 
                GLfloat ambientIntensity, GLfloat specularIntensity,GLfloat falloffC, GLfloat falloffP, GLfloat falloffQ) 
                    : ambientColor(ambientColor), color(color), pos(pos),
                    ambientIntensity(ambientIntensity), specularIntensity(specularIntensity), 
                    falloffC(falloffC), falloffP(falloffP), falloffQ(falloffQ) { };
    }
}