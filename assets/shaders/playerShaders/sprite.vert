#version 330 core
layout (location = 0) in mat4 aModel;
layout (location = 4) in vec2 aTexUV;
layout (location = 5) in vec2 aUVSize;

out VS_OUT {
    vec2 uv;
    vec2 uvSize;
    mat4 model;
} vs_out;

void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    vs_out.uv = aTexUV;
    vs_out.uvSize = aUVSize;
    vs_out.model = aModel;
}
