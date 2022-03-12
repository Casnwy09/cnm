#include <string.h>
#include <stdio.h>
#include <glad/glad.h>
#include "window.h"

static Window * windowCurrent;

static void windowResizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    windowCurrent->width = width;
    windowCurrent->height = height;
    if (windowCurrent->resizeCallback)
        windowCurrent->resizeCallback(window, width, height);
}

bool windowInit(Window * window, int width, int height, const char * title, GLFWwindowsizefun resizeCallback) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window->internal = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window->internal) {
        printf("windowInit: Couldn't create the GLFW window!\n");
        return false;
    }
    glfwMakeContextCurrent(window->internal);
    windowCurrent = window;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("windowInit: Couldn't load the glad loader!\n");
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window->internal, windowResizeCallback);

    window->width = width;
    window->height = height;
    strncpy(window->title, title, sizeof(window->title));
    window->resizeCallback = resizeCallback;
    return true;
}
void windowFree(Window * window) {
    glfwDestroyWindow(window->internal);
}
void windowUseDefaultFramebuffer(Window * window) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window->width, window->height);
}
void windowSetCurrent(Window * window) {
    glfwMakeContextCurrent(window->internal);
    windowCurrent = window;
}
