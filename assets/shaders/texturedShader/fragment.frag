#version 330 core

out vec4 FragColor;
in vec2 texUV;

uniform sampler2D primaryTex;

void main() {
    FragColor = texture(primaryTex, texUV);
}
