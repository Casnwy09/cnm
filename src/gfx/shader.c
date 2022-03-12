#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "../util/util.h"

GLuint shaderProgramFromFile(const char * vertFile, const char * geomFile, const char * fragFile) {
    GLuint program, vert, frag, geom;
    GLint status;
    char log[1024], * source;

    if (!(source = fileToUTF8(vertFile))) {
        printf("shaderProgramFromFile: Couldn't load the file \"%s\"!\n", vertFile);
        return 0;
    }
    vert = shaderFromString(GL_VERTEX_SHADER, source);
    free(source);
    if (geomFile) {
        if (!(source = fileToUTF8(geomFile))) {
            printf("shaderProgramFromFile: Couldn't load the file \"%s\"!\n", geomFile);
            glDeleteShader(vert);
            return 0;
        }
        geom = shaderFromString(GL_GEOMETRY_SHADER, source);
        free(source);
    }
    if (!(source = fileToUTF8(fragFile))) {
        printf("shaderProgramFromFile: Couldn't load the file \"%s\"!\n", fragFile);
        glDeleteShader(vert);
        if (geomFile) glDeleteShader(geom);
        return 0;
    }
    frag = shaderFromString(GL_FRAGMENT_SHADER, source);
    free(source);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    if (geomFile) glAttachShader(program, geom);
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
void texturedShaderInit(TexturedShader * shader, const char * vert, const char * geom, const char * frag) {
    shader->shader = shaderProgramFromFile(vert, geom, frag);
    shader->texture0 = glGetUniformLocation(shader->shader, "primaryTex");
    shader->texture1 = glGetUniformLocation(shader->shader, "maskTex");
    shader->projection = glGetUniformLocation(shader->shader, "projection");
    shader->view = glGetUniformLocation(shader->shader, "view");
    shader->mainColor = glGetUniformLocation(shader->shader, "mainColor");
    shader->model = glGetUniformLocation(shader->shader, "model");
}
void spriteShaderInit(SpriteShader * shader) {
    shader->shader = shaderProgramFromFile("assets/shaders/spriteShader/vertex.vert", "assets/shaders/spriteShader/geometry.geom", "assets/shaders/spriteShader/fragment.frag");
    shader->texture0 = glGetUniformLocation(shader->shader, "primaryTex");
    shader->projection = glGetUniformLocation(shader->shader, "projection");
    shader->view = glGetUniformLocation(shader->shader, "view");
}
void flatShaderInit(FlatShader * shader) {
    shader->shader = shaderProgramFromFile("assets/shaders/flatShader/vertex.vert", NULL, "assets/shaders/flatShader/fragment.frag");
    shader->mainColor = glGetUniformLocation(shader->shader, "mainColor");
    glUniform4f(shader->mainColor, 1.0f, 1.0f, 1.0f, 1.0f); // Default white
    shader->projection = glGetUniformLocation(shader->shader, "projection");
    shader->view = glGetUniformLocation(shader->shader, "view");
    shader->model = glGetUniformLocation(shader->shader, "model");
}
void outlineSpriteShaderInit(OutlineSpriteShader * shader) {
    shader->shader = shaderProgramFromFile("assets/shaders/outline/vertex.vert", NULL, "assets/shaders/outline/fragment.frag");
    shader->outlineColor = glGetUniformLocation(shader->shader, "outlineColor");
    glUniform4f(shader->outlineColor, 0.07f, 0.03f, 0.05f, 1.0f); // Default black
    //shader->model = glGetUniformLocation(shader->shader, "scale");
    shader->texture0 = glGetUniformLocation(shader->shader, "primaryTex");
    shader->pixelStep = glGetUniformLocation(shader->shader, "pixelStep");
}
