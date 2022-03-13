#ifndef _texture_h_
#define _texture_h_
#include <glad/glad.h>
#include <cglm/types-struct.h>
#include "shader.h"

typedef struct Texture {
    GLuint texID;
    int width, height;
} Texture;

void textureLoad(Texture * tex, const char * file);
void textureActivate(Texture * tex, UniformLoc sampler2D, int textureSlot);
vec2s texturePixelToUV(Texture * tex, float px, float py);

#endif
