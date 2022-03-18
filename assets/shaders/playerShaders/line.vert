#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;

out vec2 texUV;

uniform mat4 projection, view, model;

void main() {
    //float texsize = 64;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = round(gl_Position * texsize) / texsize;
    texUV = aTexUV;
}
