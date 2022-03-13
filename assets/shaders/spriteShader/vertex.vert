#version 330 core
layout (location = 0) in mat4 aModel;
layout (location = 4) in vec2 aTopLeftUV;
layout (location = 5) in vec2 aBottomRightUV;

out VS_OUT {
    vec2 uvTopLeft;
    vec2 uvBottomRight;
    mat4 model;
} vs_out;

void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    vs_out.uvTopLeft = aTopLeftUV;
    vs_out.uvBottomRight = aBottomRightUV;
    vs_out.model = aModel;
}
