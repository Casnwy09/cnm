#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec2 uvTopLeft;
    vec2 uvBottomRight;
    mat4 model;
} gs_in[];

out vec2 texUV;

uniform mat4 projection, view;

void main() {
    //float texsize = 32;
    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(-.5, -.5, 0, 0));
    //gl_Position = floor(gl_Position * texsize) / texsize;
    texUV = vec2(gs_in[0].uvTopLeft.x, gs_in[0].uvBottomRight.y);
    EmitVertex();

    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(.5, -.5, 0, 0));
    //gl_Position = floor(gl_Position * texsize) / texsize;
    texUV = vec2(gs_in[0].uvBottomRight.x, gs_in[0].uvBottomRight.y);
    EmitVertex();

    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(-.5, .5, 0, 0));
    //gl_Position = floor(gl_Position * texsize) / texsize;
    texUV = vec2(gs_in[0].uvTopLeft.x, gs_in[0].uvTopLeft.y);
    EmitVertex();

    gl_Position = projection * view * gs_in[0].model * (gl_in[0].gl_Position + vec4(.5, .5, 0, 0));
    //gl_Position = floor(gl_Position * texsize) / texsize;
    texUV = vec2(gs_in[0].uvBottomRight.x, gs_in[0].uvTopLeft.y);
    EmitVertex();

    EndPrimitive();
}
