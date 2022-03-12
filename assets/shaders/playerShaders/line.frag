#version 330 core

out vec4 FragColor;
in vec2 texUV;

uniform vec4 mainColor;
uniform sampler2D primaryTex, maskTex;

void main() {
    vec4 color = vec4(1.0);
    if (texture(maskTex, texUV).x > 0.5)
        color = mainColor;
    FragColor = texture(primaryTex, texUV) * color;
}
