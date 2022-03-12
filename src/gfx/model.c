#include "model.h"

void modelBufferVerticies(Model * model, size_t dataSize, const void * data, GLenum usage) {
    glBindVertexArray(model->vao);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
}
void modelBufferIndicies(Model * model, size_t dataSize, const void * data, GLenum usage) {
    glBindVertexArray(model->vao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);
}

void modelInit(Model * model) {
    glGenBuffers(2, &model->vbo);       // Generate vbo and ebo
    glGenVertexArrays(1, &model->vao);
    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
}
void modelFree(Model * model) {
    glDeleteVertexArrays(1, &model->vao);
    glDeleteBuffers(2, &model->vbo);
}

void spriteModelInit(Model * spriteModel) {
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

void createQuadModel(Model * model, vec4 uvs) {
    static const VertIndex indicies[] = {
        2, 0, 1,    2, 1, 3
    };
    Vertex verticies[] = {
        // x, y, z,   u, v
        { .pos = {{-0.5f,  0.5f, 0.0f}}, .uv = {{uvs[0], uvs[2]}}, .color = {{1.0f, 1.0f, 1.0f, 1.0f}} },       // Top left
        { .pos = {{ 0.5f,  0.5f, 0.0f}}, .uv = {{uvs[1], uvs[2]}}, .color = {{1.0f, 1.0f, 1.0f, 1.0f}} },       // Top right
        { .pos = {{-0.5f, -0.5f, 0.0f}}, .uv = {{uvs[0], uvs[3]}}, .color = {{1.0f, 1.0f, 1.0f, 1.0f}} },    // Bottom left
        { .pos = {{ 0.5f, -0.5f, 0.0f}}, .uv = {{uvs[1], uvs[3]}}, .color = {{1.0f, 1.0f, 1.0f, 1.0f}} },    // Bottom right
    };

    modelInit(model);
    modelBufferVerticies(model, sizeof(verticies), verticies, GL_STATIC_DRAW);
    modelBufferIndicies(model, sizeof(indicies), indicies, GL_STATIC_DRAW);
    model->numIndicies = 6;
    model->numVerticies = 4;
}
void createPointModel(Model * model) {
    static const VertIndex indicies[] = { 0 };
    static const Vertex verticies = { .pos = {{ 0.0f, 0.0f, 0.0f}}, .uv = {{0.0f, 0.0f}}, .color = {{1.0f, 1.0f, 1.0f, 1.0f}} };

    modelInit(model);
    modelBufferVerticies(model, sizeof(verticies), &verticies, GL_STATIC_DRAW);
    modelBufferIndicies(model, sizeof(indicies), indicies, GL_STATIC_DRAW);
    model->numIndicies = 1;
    model->numVerticies = 1;
}
