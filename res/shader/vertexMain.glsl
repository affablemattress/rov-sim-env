#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexTexCoord;

uniform vec4 vertexColors[4];
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 vertexColor;
out vec2 texCoord;

void main() {
    vertexColor = vertexColors[gl_VertexID % 4];
    
    texCoord = vertexTexCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.f);
}