#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;

uniform sampler2D diffuseMap;

uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

uniform vec3 pointLightColor;
uniform vec3 pointLightPos;

uniform int specularShininess;
uniform float specularStrength;

uniform vec3 cameraPos;

out vec4 fragColor;

void main() {
    vec3 lightDirection = normalize(pointLightPos - fragPos);
    vec3 reflectionDirecion = reflect(-lightDirection, fragNormal);
    vec3 viewDirection = normalize(cameraPos - fragPos);

    vec3 ambientLight = ambientLightColor * ambientLightIntensity;

    float diffuseLightIntensity = max(dot(fragNormal, lightDirection), 0.f);
    vec3 diffuseLight = pointLightColor * diffuseLightIntensity;

    float specularIntensity = pow(max(dot(viewDirection, reflectionDirecion), 0.0), specularShininess) * specularStrength;
    vec3 specularLight = specularIntensity * pointLightColor;

    fragColor = vec4(specularLight + ambientLight + diffuseLight, 1.f) * texture(diffuseMap, fragUV);
}