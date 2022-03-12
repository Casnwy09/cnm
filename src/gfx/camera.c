#include <glad/glad.h>
#include "camera.h"

void cameraUpdateMatrix(Camera * cam) {
    glm_translate_make(cam->mats.viewInv, (vec3){cam->x, cam->y, 0.0f});
    glm_rotate_z(cam->mats.viewInv, cam->ang, cam->mats.viewInv);
    glm_scale(cam->mats.viewInv, (vec3){1.0f / cam->zoom, 1.0f / cam->zoom, 1.0f});
    glm_mat4_inv(cam->mats.viewInv, cam->mats.view);
}
void cameraUpdateFromResize(Camera * cam, int newWindowWidth, int newWindowHeight) {
    glm_ortho_default((float)newWindowWidth / (float)newWindowHeight, cam->mats.proj);
    cam->sw = 1.0f / cam->mats.proj[0][0];       // Gets scale on x
    cam->sh = 1.0f / cam->mats.proj[1][1];       // Gets scale on y
    glm_mat4_inv(cam->mats.proj, cam->mats.projInv);
}
void cameraUpdateShaderUniforms(const Camera * cam, UniformLoc proj, UniformLoc view) {
    glUniformMatrix4fv(proj, 1, GL_FALSE, (void *)cam->mats.proj);
    glUniformMatrix4fv(view, 1, GL_FALSE, (void *)cam->mats.view);
}
vec2s cameraGetWorldCoordsFromMouse(Camera * cam, GLFWwindow * window) {
    alignas(16) mat4 m;
    int w, h;
    double mx, my;

    glfwGetWindowSize(window, &w, &h);
    glfwGetCursorPos(window, &mx, &my);

    mx = (mx / (double)w) * 2.0f - 1.0f;
    my = (my / (double)h) * -2.0f + 1.0f;

    vec4s mousePos = (vec4s){{(float)mx, (float)my, 0.0f, 1.0f}};
    glm_mat4_mul(cam->mats.projInv, cam->mats.viewInv, m);
    glm_mat4_mulv(m, mousePos.raw, mousePos.raw);
    return (vec2s){{mousePos.x, mousePos.y}};
}
