#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

uniform float mixWeight;
uniform sampler2D texture0;

out vec4 fragColor;

void main() {
    fragColor = mix(vertexColor, texture(texture0, texCoord), mixWeight);
}