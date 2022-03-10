#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/texture.h"

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    Model m;
    modelInit(&m);
    modelGenerateQuad(&m, 0.0f, 1.0f, 0.0f, 1.0f, GL_STATIC_DRAW);
    Shader s = shaderProgramFromFile("assets/shaders/test.vert", "assets/shaders/test.frag");
    Texture t = textureLoad("assets/textures/brickTest.png");

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(s);
        glUniform1i(glGetUniformLocation(s, "primaryTex"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t);
        glBindVertexArray(m.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    modelFree(&m);
    glDeleteProgram(s);
    glDeleteTextures(1, &t);

    glfwTerminate();
    return 0;
}
