#ifndef _shader_h_
#define _shader_h_
#include <glad/glad.h>

typedef GLuint Shader;
typedef GLint UniformLoc;

typedef struct GenericShader {
    Shader shader;
    UniformLoc mainColor;
    UniformLoc texture0;
    UniformLoc projection;
    UniformLoc view;
    UniformLoc model;
} GenericShader;
typedef GenericShader TexturedShader;
typedef GenericShader SpriteShader;
typedef GenericShader FlatShader;

GLuint shaderProgramFromFile(const char * vertFile, const char * geomFile, const char * fragFile);
GLuint shaderFromString(GLenum type, const char * source);

void texturedShaderInit(TexturedShader * shader);
void spriteShaderInit(SpriteShader * shader);
void flatShaderInit(FlatShader * shader);

#endif
