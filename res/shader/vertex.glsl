#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

uniform vec2 position;
uniform vec4 vertexColors[4];

void main() {
    vertexColor = vertexColors[gl_VertexID % 4];
    
    texCoord = vertexTexCoord;
    gl_Position = vec4(vertexPos.x + position.x, vertexPos.y + position.y, vertexPos.z, 1.0);
}