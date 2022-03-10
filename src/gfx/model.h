#ifndef _model_h_
#define _model_h_
#include <stddef.h>
#include <glad/glad.h>

typedef unsigned int VertIndex;
typedef struct Vertex {
    float x, y, z;
    float u, v;
} Vertex;
typedef struct Model {
    GLuint vao, vbo, ebo;
} Model;

void modelInit(Model * model);
void modelFree(Model * model);
void modelBufferData(
    Model * model, size_t numVerticies, Vertex * verticies,
    size_t numIndicies, VertIndex * indicies, GLenum usage
);
void modelGenerateQuad(
    Model * model,
    float uLeft, float uRight, float vBottom, float vTop,
    GLenum usage
);
void modelGeneratePoint(
    Model * model, GLenum usage
);

#endif
