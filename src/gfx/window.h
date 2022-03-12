#ifndef _window_h_
#define _window_h_
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct Window {
    GLFWwindow * internal;

    GLFWwindowsizefun resizeCallback;
    char title[1024];
    int width, height;
} Window;

bool windowInit(Window * window, int width, int height, const char * title, GLFWwindowsizefun resizeCallback);
void windowFree(Window * window);
void windowUseDefaultFramebuffer(Window * window);
void windowSetCurrent(Window * window);

#endif
