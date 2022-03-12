#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec2 uv;
    vec2 uvSize;
    mat4 model;
} gs_in[];

out vec2 texUV;

uniform mat4 projection, view;

void main() {
    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(0, 0, 0, 0));
    texUV = gs_in[0].uv + (vec2(0.0, 0.0) * gs_in[0].uvSize);
    EmitVertex();

    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(.5, 0, 0, 0));
    texUV = gs_in[0].uv + (vec2(1.0, 0.0) * gs_in[0].uvSize);
    EmitVertex();

    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(0, .5, 0, 0));
    texUV = gs_in[0].uv + (vec2(0.0, 1.0) * gs_in[0].uvSize);
    EmitVertex();
    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(.5, .5, 0, 0));
    texUV = gs_in[0].uv + (vec2(1.0, 1.0) * gs_in[0].uvSize);
    EmitVertex();

    EndPrimitive();
}
