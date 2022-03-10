#ifndef _camera_h_
#define _camera_h_
#include <stdalign.h>
#include <cglm/cglm.h>
#include "shader.h"

typedef struct Camera {
    alignas(16) float x, y;
    float ang;
    float zoom;

    float sw, sh;                 // These are read only values

    struct {
        alignas(16) mat4 proj;
        alignas(16) mat4 view;
    } mats;
} Camera;

void cameraUpdateMatrix(Camera * cam);
void cameraUpdateFromResize(Camera * cam, int newWindowWidth, int newWindowHeight);
void cameraUpdateShaderUniforms(const Camera * cam, UniformLoc proj, UniformLoc view);

#endif
