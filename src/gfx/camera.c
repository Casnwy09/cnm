#include <glad/glad.h>
#include "camera.h"

void cameraUpdateMatrix(Camera * cam) {
    glm_translate_make(cam->mats.view, (vec3){cam->x, cam->y, 0.0f});
    glm_rotate_z(cam->mats.view, cam->ang, cam->mats.view);
    glm_scale(cam->mats.view, (vec3){1.0f / cam->zoom, 1.0f / cam->zoom, 1.0f});
    glm_mat4_inv(cam->mats.view, cam->mats.view);
}
void cameraUpdateFromResize(Camera * cam, int newWindowWidth, int newWindowHeight) {
    glm_ortho_default((float)newWindowWidth / (float)newWindowHeight, cam->mats.proj);
    cam->sw = 1.0f / cam->mats.proj[0][0];       // Gets scale on x
    cam->sh = 1.0f / cam->mats.proj[1][1];       // Gets scale on y
}
void cameraUpdateShaderUniforms(const Camera * cam, UniformLoc proj, UniformLoc view) {
    glUniformMatrix4fv(proj, 1, GL_FALSE, (void *)cam->mats.proj);
    glUniformMatrix4fv(view, 1, GL_FALSE, (void *)cam->mats.view);
}
