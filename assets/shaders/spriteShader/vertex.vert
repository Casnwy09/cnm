#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;

out VS_OUT {
    vec2 uv;
} vs_out;

uniform mat4 projection, view, model;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    vs_out.uv = aTexUV;
}
