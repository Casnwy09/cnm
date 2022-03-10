#ifndef _texture_h_
#define _texture_h_
#include <glad/glad.h>

typedef GLuint Texture;

Texture textureLoad(const char * file);

#endif
