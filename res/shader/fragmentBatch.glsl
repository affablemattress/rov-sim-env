#version 330 core

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

layout (std140) uniform ActiveLights {
    PointLight pointLights[16];
    int pointLightCount;
} activeLights;

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

in vec4 vertexColor;

uniform float mixWeight;
uniform sampler2D diffuseMap;

out vec4 fragColor;

void main() {
    float specularShininess = 32.0;
    
    vec3 viewDir = normalize(cameraData.pos - fragData.pos);
    vec3 lightAccum = vec3(0.0, 0.0, 0.0);
    
    for (int i = 0; i < 16; i++) {
        if (i == activeLights.pointLightCount) break;

        vec3 lightDir = normalize(activeLights.pointLights[i].pos - fragData.pos);
        vec3 reflectionDir = reflect(-lightDir, fragData.normal);
        
        vec3 ambientComponent = activeLights.pointLights[i].ambientColor * activeLights.pointLights[i].ambientIntensity;

        float diffuseIntensity = max(dot(fragData.normal, lightDir), 0.0);
        vec3 diffuseComponent = activeLights.pointLights[i].color * diffuseIntensity;
        
        float specularIntensity = pow(max(dot(viewDir, reflectionDir), 0.0), specularShininess) * activeLights.pointLights[i].specularIntensity;
        vec3 specularComponent = activeLights.pointLights[i].color * specularIntensity;

        lightAccum += (specularComponent + ambientComponent + diffuseComponent);
    }

    fragColor = vec4(lightAccum, 1.f) * mix(vertexColor, texture(diffuseMap, fragData.UV), mixWeight);
}