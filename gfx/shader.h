#ifndef _shader_h_
#define _shader_h_
#include <glad/glad.h>

typedef GLuint Shader;

GLuint shaderProgramFromFile(const char * vertFile, const char * fragFile);
GLuint shaderFromString(GLenum type, const char * source);

#endif
