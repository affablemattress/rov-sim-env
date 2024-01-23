#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 2) in vec2 vertexTexCoord;

uniform vec4 mixColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 vertexColor;
out vec2 texCoord;

void main() {
    vertexColor = mixColor;
    texCoord = vertexTexCoord;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.f);
}