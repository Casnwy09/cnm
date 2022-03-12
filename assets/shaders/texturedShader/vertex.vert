#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexUV;
//layout (location = 2) in vec4 aColor;

out vec2 texUV;
//out vec4 color;

uniform mat4 projection, view, model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texUV = aTexUV;
    //color = aColor;
}
