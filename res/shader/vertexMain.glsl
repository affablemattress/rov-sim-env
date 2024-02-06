#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

layout (std140) uniform CameraData {
    mat4 viewMatrix;
    mat4 projectionMatrix;

    vec3 pos;
} cameraData;

layout (std140) uniform ModelData {
    mat4 modelMatrix;
    mat4 normalMatrix;
} modelData;

out FragmentData{
    vec3 pos;
    vec3 normal;
    vec2 UV;
} fragData;

void main() {
    gl_Position = cameraData.projectionMatrix * cameraData.viewMatrix * modelData.modelMatrix * vec4(vertexPos, 1.f);

    fragData.pos = vec3(modelData.modelMatrix * vec4(vertexPos, 1.f));
    fragData.normal = normalize(mat3(modelData.normalMatrix) * vertexNormal);
    fragData.UV = vertexUV;
}