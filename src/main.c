#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/texture.h"
#include "gfx/camera.h"
#include "util/util.h"
#include "gfx/lineRenderer.h"
#include "util/math.h"

static Camera camera;

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    cameraUpdateFromResize(&camera, width, height);
}

int main(int argc, char ** argv) {
    if (!glfwInit()) {
        printf("main: Couldn't initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "cnm", NULL, NULL);
    if (!window) {
        printf("main: Couldn't create the GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("main: Couldn't load the glad loader!\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    cameraUpdateFromResize(&camera, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Model m;
    //spriteModelInit(&m);
    //SpriteShader shader;
    //spriteShaderInit(&shader);

    TexturedShader shader;
    texturedShaderInit(&shader, "assets/shaders/playerShaders/line.vert", NULL, "assets/shaders/playerShaders/line.frag");
    vec4s shaderColor = (vec4s){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};

    Texture t[2];
    textureLoad(t + 0, "assets/textures/player.png");
    textureLoad(t + 1, "assets/textures/playerMask.png");
    LineRenderer line;
    lineRendererCreate(&line, 3);

    //SpriteVertexEntry sprites[3] = {
    //    {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}},
    //    {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}},
    //    {.uvPos = {0.0f, 0.0f}, .uvSize = {1.0f, 1.0f}}
    //};
    //generateModelMatrix(sprites[0].model, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    //generateModelMatrix(sprites[1].model, -0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 45.0f / 180.0f * 3.1415962f);
    //generateModelMatrix(sprites[2].model, 0.0f, -0.25f, 0.0f, 2.0f, 1.0f, 0.0f);
    //modelBufferVerticies(&m, sizeof(sprites), sprites, GL_STATIC_DRAW);
    //m.numVerticies = sizeof(sprites) / sizeof(SpriteVertexEntry);

    camera.x = 0.0f;
    camera.y = 0.0f;
    camera.zoom = 1.0f/20.0f;
    camera.ang = 0.0f;
    cameraUpdateMatrix(&camera);

    line.segments[0].pos = (vec2s){ .x = -10.0f, .y = 0.0f };
    line.segments[0].uv1 = texturePixelToUV(t+0, 16, 14);
    line.segments[0].uv2 = texturePixelToUV(t+0, 18, 14);
    line.segments[1].pos = (vec2s){ .x = 0.0f, .y = 0.0f };
    line.segments[1].uv1 = texturePixelToUV(t+0, 16, 23);
    line.segments[1].uv2 = texturePixelToUV(t+0, 18, 23);
    line.segments[2].pos = (vec2s){ .x = 10.0f, .y = 0.0f };
    line.segments[2].uv1 = texturePixelToUV(t+0, 16, 32);
    line.segments[2].uv2 = texturePixelToUV(t+0, 18, 32);

    int grabMode = -1;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (glfwGetKey(window, GLFW_KEY_W))
            camera.y += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_S))
            camera.y -= 0.1f;

        if (glfwGetKey(window, GLFW_KEY_U) && shaderColor.x < 1.0f) shaderColor.x += 0.02f;
        if (glfwGetKey(window, GLFW_KEY_J) && shaderColor.x > 0.0f) shaderColor.x -= 0.02f;
        if (glfwGetKey(window, GLFW_KEY_I) && shaderColor.y < 1.0f) shaderColor.y += 0.02f;
        if (glfwGetKey(window, GLFW_KEY_K) && shaderColor.y > 0.0f) shaderColor.y -= 0.02f;
        if (glfwGetKey(window, GLFW_KEY_O) && shaderColor.z < 1.0f) shaderColor.z += 0.02f;
        if (glfwGetKey(window, GLFW_KEY_L) && shaderColor.z > 0.0f) shaderColor.z -= 0.02f;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
            vec2s mpos = cameraGetWorldCoordsFromMouse(&camera, window);

            for (int i = 0; i < line.numSegments && grabMode == -1; i++) {
                if (glm_vec2_distance(mpos.raw, line.segments[i].pos.raw) < line.segments[i].size / 4.0f * 3.0f) {
                    grabMode = i;
                    break;
                }
            }

            int closestIndex = line.numSegments - 1;
            float closestDist = FLT_MAX;
            if (grabMode == -1) {
                for (int i = 0; i < line.numSegments; i++) {
                    float dist = glm_vec2_distance(mpos.raw, line.segments[i].pos.raw);
                    if (dist < closestDist) {
                        closestIndex = i;
                        closestDist = dist;
                    }
                }

                lineRendererResize(&line, line.numSegments+1);
                if (closestIndex != line.numSegments-2) {
                    memmove(
                        line.segments + closestIndex + 1,
                        line.segments + closestIndex,
                        sizeof(LineRendererSegment) * (line.numSegments - closestIndex - 1)
                    );
                }
                line.segments[closestIndex].color = (vec4s){{0.0f, 0.0f, 1.0f, 1.0f}};
                line.segments[closestIndex].pos = mpos;
                line.segments[closestIndex].size = 1.0f;
                grabMode = closestIndex;
            }
        }
        if (grabMode > -1) {
            line.segments[grabMode].pos = cameraGetWorldCoordsFromMouse(&camera, window);
            if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) grabMode = -1;
        }
        lineRendererUpdateModel(&line);

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cameraUpdateMatrix(&camera);
        glUseProgram(shader.shader);
        glUniform4f(shader.mainColor, shaderColor.x, shaderColor.y, shaderColor.z, shaderColor.w);
        textureActivate(t+0, shader.texture0, 0);
        textureActivate(t+1, shader.texture1, 1);
        cameraUpdateShaderUniforms(&camera, shader.projection, shader.view);
        lineRendererRender(&line);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //modelFree(&m);
    lineRendererDestroy(&line);
    glDeleteProgram(shader.shader);
    glDeleteTextures(1, &t[0].texID);
    glDeleteTextures(1, &t[1].texID);

    glfwTerminate();
    return 0;
}
