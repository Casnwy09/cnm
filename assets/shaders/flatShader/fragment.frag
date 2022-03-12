#version 330 core

out vec4 FragColor;
in vec2 texUV;
in vec4 color;

uniform vec4 mainColor;

void main() {
    FragColor = mainColor * color;
}
