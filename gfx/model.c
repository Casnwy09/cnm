#include "model.h"

void modelInit(Model * model) {
    if (!model) return;

    glGenBuffers(2, &model->vbo);       // Generate vbo and ebo
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
void modelFree(Model * model) {
    glDeleteVertexArrays(1, &model->vao);
    glDeleteBuffers(2, &model->vbo);
}
void modelBufferData(
    Model * model, size_t numVerticies, Vertex * verticies,
    size_t numIndicies, VertIndex * indicies, GLenum usage) {
    glBindVertexArray(model->vao);
    glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vertex), verticies, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies * sizeof(VertIndex), indicies, usage);
}
void modelGenerateQuad(
    Model * model,
    float uLeft, float uRight, float vBottom, float vTop,
    GLenum usage) {
    static const VertIndex indicies[] = {
        2, 0, 1,    2, 1, 3
    };
    float verticies[] = {
        // x, y, z,   u, v
        -0.5f,  0.5f, 0.0f, uLeft,  vTop,       // Top left
         0.5f,  0.5f, 0.0f, uRight, vTop,       // Top right
        -0.5f, -0.5f, 0.0f, uLeft,  vBottom,    // Bottom left
         0.5f, -0.5f, 0.0f, uRight, vBottom,    // Bottom right
    };

    glBindVertexArray(model->vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, usage);
}
