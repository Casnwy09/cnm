#ifndef _model_h_
#define _model_h_
#include <stddef.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stdalign.h>

typedef unsigned int VertIndex;
typedef struct Vertex {
    float x, y, z;
    float u, v;
} Vertex;
typedef struct SpriteVertexEntry {
    alignas(16) mat4 model;
    vec2 uvPos;
    vec2 uvSize;
} SpriteVertexEntry;
typedef struct Model {
    GLuint vao, vbo, ebo;
    int numIndicies, numVerticies;
} Model;

void modelBufferVerticies(Model * model, size_t dataSize, const void * data, GLenum usage);
void modelBufferIndicies(Model * model, size_t dataSize, const void * data, GLenum usage);

void modelInit(Model * model);
void modelFree(Model * model);

void spriteModelInit(Model * spriteModel);

void modelRender(Model * model, GLenum mode);
void spriteModelRender(Model * spriteModel);

void createQuadModel(Model * model, vec4 uvs);
void createPointModel(Model * model);

#endif
