#version 330 core

/*
struct DiralLight {
    vec3 ambientColor;
    vec3 color;

    vec3 direction;

    float ambientIntensity;
    float specularIntensity;
};

struct SpotLight {
    vec3 color;

    vec3 pos;
    vec3 dir;

    float specularIntensity;

    float coneAlpha;
    float coneBeta;

    float falloffC;
    float falloffP;
    float falloffQ;
};

struct AmbientLight {
    vec3 ambientColor;
    float ambientIntensity;
};
*/

struct PointLight {
    vec3 ambientColor;
    vec3 color;

    vec3 pos;

    float ambientIntensity;
    float specularIntensity;

    float falloffC;
    float falloffP;
    float falloffQ;
};

layout (std140) uniform LightData {
    PointLight pointLights[16];
    int pointLightCount;
} lightData;

layout (std140) uniform CameraData {
    mat4 viewMatrix;
    mat4 projectionMatrix;

    vec3 pos;
} cameraData;

in FragmentData{
    vec3 pos;
    vec3 normal;
    vec2 UV;
} fragData;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform float specularShininess;

out vec4 fragColor;

void main() {
    vec3 viewDir = normalize(cameraData.pos - fragData.pos);
    vec3 lightAccum = vec3(0.0, 0.0, 0.0);
    
    for (int i = 0; i < 16; i++) {
        if (i == lightData.pointLightCount) break;

        vec3 lightDir = normalize(lightData.pointLights[i].pos - fragData.pos);
        vec3 reflectionDir = reflect(-lightDir, fragData.normal);
        
        vec3 ambientComponent = lightData.pointLights[i].ambientColor * lightData.pointLights[i].ambientIntensity;

        float diffuseIntensity = max(dot(fragData.normal, lightDir), 0.0);
        vec3 diffuseComponent = lightData.pointLights[i].color * diffuseIntensity;
        
        float specularIntensity = pow(max(dot(viewDir, reflectionDir), 0.0), specularShininess) * lightData.pointLights[i].specularIntensity;
        vec3 specularComponent = lightData.pointLights[i].color * specularIntensity * vec3(texture(specularMap, fragData.UV));

        lightAccum += (specularComponent + ambientComponent + diffuseComponent);
    }

    fragColor = vec4(lightAccum, 1.f) * texture(diffuseMap, fragData.UV);
}