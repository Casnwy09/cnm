#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "../util/util.h"

GLuint shaderProgramFromFile(const char * vertFile, const char * fragFile) {
    GLuint program, vert, frag;
    GLint status;
    char log[1024], * source;

    if (!(source = fileToUTF8(vertFile))) return 0;
    vert = shaderFromString(GL_VERTEX_SHADER, source);
    free(source);
    if (!(source = fileToUTF8(fragFile))) { glDeleteShader(vert); return 0; }
    frag = shaderFromString(GL_FRAGMENT_SHADER, source);
    free(source);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("SHADER PROGRAM LINK ERROR: %s\n", log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    glUseProgram(program);
    return program;
}
GLuint shaderFromString(GLenum type, const char * source) {
    GLuint shader;
    GLint status;
    char log[1024];

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("SHADER COMPILE ERROR: %s\n", log);
    }

    return shader;
}
