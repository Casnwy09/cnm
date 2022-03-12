#version 330 core

out vec4 FragColor;
in vec2 texUV;

uniform sampler2D primaryTex;
uniform vec2 pixelStep;
uniform vec4 outlineColor;

void main() {
    FragColor = texture(primaryTex, texUV);
    if (FragColor.a < 0.1 && (
        texture(primaryTex, texUV + vec2(pixelStep.x, 0)).a > 0.1 ||
        texture(primaryTex, texUV - vec2(pixelStep.x, 0)).a > 0.1 ||
        texture(primaryTex, texUV + vec2(0, pixelStep.y)).a > 0.1 ||
        texture(primaryTex, texUV - vec2(0, pixelStep.y)).a > 0.1)) {
        FragColor = outlineColor;
    }
}
