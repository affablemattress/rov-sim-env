#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform float mixWeight;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    fragColor = mix(vertexColor, texture(texture0, texCoord), mixWeight);
}