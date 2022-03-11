#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/texture.h"
#include "gfx/camera.h"
#include "util/util.h"

static Camera camera;

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    cameraUpdateFromResize(&camera, width, height);
}

int main(int argc, char ** argv) {
    if (!glfwInit()) {
        printf("main: Couldn't initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "cnm", NULL, NULL);
    if (!window) {
        printf("main: Couldn't create the GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("main: Couldn't load the glad loader!\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    cameraUpdateFromResize(&camera, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    Model m;
    spriteModelInit(&m);
    SpriteShader shader;
    spriteShaderInit(&shader);
    Texture t = textureLoad("assets/textures/brickTest.png");

    SpriteVertexEntry sprites[3] = {
        {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}},
        {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}},
        {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}}
    };
    generateModelMatrix(sprites[0].model, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    generateModelMatrix(sprites[1].model, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 45.0f / 180.0f * 3.1415962f);
    generateModelMatrix(sprites[2].model, 0.0f, -0.25f, 0.0f, 2.0f, 1.0f, 0.0f);
    modelBufferVerticies(&m, sizeof(sprites), sprites, GL_STATIC_DRAW);
    m.numVerticies = sizeof(sprites) / sizeof(SpriteVertexEntry);

    camera.x = 0.0f;
    camera.y = 0.0f;
    camera.zoom = 1.0f;
    camera.ang = 0.0f;
    cameraUpdateMatrix(&camera);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (glfwGetKey(window, GLFW_KEY_W))
            camera.y += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_S))
            camera.y -= 0.01f;

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cameraUpdateMatrix(&camera);
        glUseProgram(shader.shader);
        glUniform1i(shader.texture0, 0);
        cameraUpdateShaderUniforms(&camera, shader.projection, shader.view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t);
        spriteModelRender(&m);
        //glBindVertexArray(m.vao);
        //glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, NULL);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    modelFree(&m);
    glDeleteProgram(shader.shader);
    glDeleteTextures(1, &t);

    glfwTerminate();
    return 0;
}
