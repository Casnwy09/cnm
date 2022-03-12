#include <stddef.h>
#include <stdio.h>
#include "framebuffer.h"

void framebufferCreate(Framebuffer * fb, int width, int height, bool alpha) {
    glGenFramebuffers(1, &fb->fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fb);

    glGenTextures(1, &fb->color);
    glBindTexture(GL_TEXTURE_2D, fb->color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->color, 0);

    glGenRenderbuffers(1, &fb->depthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, fb->depthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->depthStencil);

    fb->width = width;
    fb->height = height;
    fb->alpha = alpha;

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    printf("framebufferCreate: The framebuffer is incomplete!\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebufferBind(Framebuffer * fb) {
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fb);
    glViewport(0, 0, fb->width, fb->height);
}
void framebufferDestroy(Framebuffer * fb) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fb->fb);
    glDeleteTextures(1, &fb->color);
}
void framebufferActivateColorTexture(Framebuffer * fb, UniformLoc sampler2D, int textureSlot) {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, fb->color);
    glUniform1i(sampler2D, textureSlot);
}
