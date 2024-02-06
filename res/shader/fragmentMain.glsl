#version 330 core

uniform sampler2D diffuseMap;

uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

uniform vec3 pointLightColor;
uniform vec3 pointLightPos;

uniform float specularShininess;
uniform float specularStrength;

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

out vec4 fragColor;

void main() {
    vec3 lightDirection = normalize(pointLightPos - fragData.pos);
    vec3 reflectionDirection = reflect(-lightDirection, fragData.normal);
    vec3 viewDirection = normalize(cameraData.pos - fragData.pos);

    vec3 ambientLight = ambientLightColor * ambientLightIntensity;

    float diffuseLightIntensity = max(dot(fragData.normal, lightDirection), 0.0);
    vec3 diffuseLight = pointLightColor * diffuseLightIntensity;

    float specularIntensity = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularShininess) * specularStrength;
    vec3 specularLight = pointLightColor * specularIntensity;

    fragColor = vec4(specularLight + ambientLight + diffuseLight, 1.f) * texture(diffuseMap, fragData.UV);
}