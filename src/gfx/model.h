#ifndef _model_h_
#define _model_h_
#include <stddef.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stdalign.h>
#include <cglm/types-struct.h>

typedef enum VertexType {
    VT_GENERIC,
    VT_COLORED,
    VT_SPRITE_ENTRY
} VertexType;

typedef unsigned int VertIndex;
typedef struct Vertex {
    vec3s pos;
    vec2s uv;
} Vertex;
typedef struct ColoredVertex {
    vec3s pos;
    vec2s uv;
    vec4s color;
} ColoredVertex;
typedef struct SpriteVertexEntry {
    alignas(16) mat4s model;
    vec2s uvPos;
    vec2s uvSize;
} SpriteVertexEntry;
typedef struct Model {
    GLuint vao, vbo, ebo;
    int numIndicies, numVerticies;
} Model;

void modelBufferVerticies(Model * model, size_t dataSize, const void * data, GLenum usage);
void modelBufferIndicies(Model * model, size_t dataSize, const void * data, GLenum usage);

void modelInit(Model * model, VertexType vType, bool enableElementBuffer);
void modelFree(Model * model);
//void spriteModelInit(Model * spriteModel);

void modelRender(Model * model, GLenum mode);
void modelRenderVerticies(Model * model, GLenum mode);
void spriteModelRender(Model * spriteModel);

void createQuadModel(Model * model, float width, float height, float uvLeft, float uvRight, float uvTop, float uvBottom);
void createPointModel(Model * model);

#endif
