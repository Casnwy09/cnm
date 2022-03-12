#include <string.h>
#include "model.h"

static void spriteModelInit(Model * spriteModel);

void modelBufferVerticies(Model * model, size_t dataSize, const void * data, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
}
void modelBufferIndicies(Model * model, size_t dataSize, const void * data, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);
}

void modelInit(Model * model, VertexType vType, bool enableElementBuffer) {
    size_t vertexSize;
    
    memset(model, 0, sizeof(Model));
    if (vType == VT_SPRITE_ENTRY) { spriteModelInit(model); return; }
    glGenBuffers(1, &model->vbo);       // Generate vbo and ebo
    if (enableElementBuffer) glGenBuffers(1, &model->ebo);
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    if (enableElementBuffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);

    switch (vType) {
    case VT_COLORED: vertexSize = sizeof(ColoredVertex); break;
    default: vertexSize = sizeof(Vertex);
    }
    if (vType == VT_COLORED || vType == VT_GENERIC) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize, (void *)offsetof(Vertex, uv));
        glEnableVertexAttribArray(1);
        if (vType == VT_COLORED) {
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertexSize, (void *)offsetof(ColoredVertex, color));
            glEnableVertexAttribArray(2);
        }
    }
}
void modelFree(Model * model) {
    glDeleteVertexArrays(1, &model->vao);
    glDeleteBuffers(2, &model->vbo);
}

static void spriteModelInit(Model * spriteModel) {
    glGenBuffers(1, &spriteModel->vbo);       // Generate vbo
    glGenVertexArrays(1, &spriteModel->vao);
    glBindVertexArray(spriteModel->vao);
    glBindBuffer(GL_ARRAY_BUFFER, spriteModel->vbo);

    const size_t vec4Size = sizeof(vec4);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(0 * vec4Size));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(1 * vec4Size));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(2 * vec4Size));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(3 * vec4Size));
    glEnableVertexAttribArray(3);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(4 * vec4Size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertexEntry), (void *)(4 * vec4Size + sizeof(vec2)));
    glEnableVertexAttribArray(5);

    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
}

void modelRender(Model * model, GLenum mode) {
    glBindVertexArray(model->vao);
    glDrawElements(mode, model->numIndicies, GL_UNSIGNED_INT, NULL);
}
void modelRenderVerticies(Model * model, GLenum mode) {
    glBindVertexArray(model->vao);
    glDrawArrays(mode, 0, model->numVerticies);
}
void spriteModelRender(Model * spriteModel) {
    glBindVertexArray(spriteModel->vao);
    glDrawArraysInstanced(GL_POINTS, 0, 1, spriteModel->numVerticies);
}

void createQuadModel(Model * model, float width, float height, float uvLeft, float uvRight, float uvTop, float uvBottom) {
    static const VertIndex indicies[] = {
        2, 0, 1,    2, 1, 3
    };
    Vertex verticies[] = {
        // x, y, z,   u, v
        { .pos = {{-0.5f*width,  0.5f*height, 0.0f}}, .uv = {{uvLeft,  uvTop}} },       // Top left
        { .pos = {{ 0.5f*width,  0.5f*height, 0.0f}}, .uv = {{uvRight, uvTop}} },       // Top right
        { .pos = {{-0.5f*width, -0.5f*height, 0.0f}}, .uv = {{uvLeft,  uvBottom}} },    // Bottom left
        { .pos = {{ 0.5f*width, -0.5f*height, 0.0f}}, .uv = {{uvRight, uvBottom}} },    // Bottom right
    };

    modelInit(model, VT_GENERIC, true);
    modelBufferVerticies(model, sizeof(verticies), verticies, GL_STATIC_DRAW);
    modelBufferIndicies(model, sizeof(indicies), indicies, GL_STATIC_DRAW);
    model->numIndicies = 6;
    model->numVerticies = 4;
}
void createPointModel(Model * model) {
    static const VertIndex indicies[] = { 0 };
    static const Vertex verticies = { .pos = {{ 0.0f, 0.0f, 0.0f}}, .uv = {{0.0f, 0.0f}} };

    modelInit(model, VT_GENERIC, true);
    modelBufferVerticies(model, sizeof(verticies), &verticies, GL_STATIC_DRAW);
    modelBufferIndicies(model, sizeof(indicies), indicies, GL_STATIC_DRAW);
    model->numIndicies = 1;
    model->numVerticies = 1;
}
