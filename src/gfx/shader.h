#ifndef _shader_h_
#define _shader_h_
#include <glad/glad.h>

typedef GLuint Shader;
typedef GLint UniformLoc;

typedef struct GenericShader {
    Shader shader;
    union {
        UniformLoc mainColor;
        UniformLoc outlineColor;
    };
    UniformLoc texture0;
    UniformLoc texture1;
    UniformLoc projection;
    UniformLoc view;
    UniformLoc model;
    UniformLoc pixelStep;
} GenericShader;
typedef GenericShader TexturedShader;
typedef GenericShader SpriteShader;
typedef GenericShader FlatShader;
typedef GenericShader OutlineSpriteShader;

GLuint shaderProgramFromFile(const char * vertFile, const char * geomFile, const char * fragFile);
GLuint shaderFromString(GLenum type, const char * source);

void texturedShaderInit(TexturedShader * shader, const char * vert, const char * geom, const char * frag);
void spriteShaderInit(SpriteShader * shader);
void flatShaderInit(FlatShader * shader);
void outlineSpriteShaderInit(OutlineSpriteShader * shader);

#endif
