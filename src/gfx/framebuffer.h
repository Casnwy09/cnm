#ifndef _framebuffer_h_
#define _framebuffer_h_
#include <stdbool.h>
#include <glad/glad.h>
#include "shader.h"

typedef struct Framebuffer {
    GLuint fb;
    GLuint color;
    GLuint depthStencil;

    int width, height;
    bool alpha;
} Framebuffer;

void framebufferCreate(Framebuffer * fb, int width, int height, bool alpha);
void framebufferDestroy(Framebuffer * fb);
void framebufferBind(Framebuffer * fb);
void framebufferActivateColorTexture(Framebuffer * fb, UniformLoc sampler2D, int textureSlot);

#endif
