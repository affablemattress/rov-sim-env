#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform vec2 position;
uniform vec4 vertexColors[4];

void main() {
    vertexColor = vertexColors[gl_VertexID % 4];
    gl_Position = vec4(aPos.x + position.x, aPos.y + position.y, aPos.z, 1.0);
}