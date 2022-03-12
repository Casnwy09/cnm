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
#include "gfx/window.h"
#include "gfx/framebuffer.h"

static Camera camera;
static Camera spriteCamera;
static alignas(16) mat4 modelMatrix;

void framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    cameraUpdateFromResize(&camera, width, height);
}

int main(int argc, char ** argv) {
    if (!glfwInit()) {
        printf("main: Couldn't initialize GLFW!\n");
        return -1;
    }

    Window window;
    windowInit(&window, 800, 600, "cnm", framebufferSizeCallback);
    cameraUpdateFromResize(&camera, window.width, window.height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Model m;
    //spriteModelInit(&m);
    //SpriteShader shader;
    //spriteShaderInit(&shader);

    Framebuffer spriteScreen, outlineScreen;
    framebufferCreate(&spriteScreen, 32, 32, true);
    framebufferCreate(&outlineScreen, spriteScreen.width + 2, spriteScreen.height + 2, true);
    cameraUpdateFromResize(&spriteCamera, spriteScreen.width, spriteScreen.height);
    spriteCamera.x = 0.0f;
    spriteCamera.y = 0.0f;
    spriteCamera.ang = 0.0f;
    spriteCamera.zoom = 1.0f / 20.0f;
    cameraUpdateMatrix(&spriteCamera);

    TexturedShader shader, shader2;
    OutlineSpriteShader outlineShader;
    texturedShaderInit(&shader2, "assets/shaders/texturedShader/vertex.vert", NULL, "assets/shaders/texturedShader/fragment.frag");
    texturedShaderInit(&shader, "assets/shaders/playerShaders/line.vert", NULL, "assets/shaders/playerShaders/line.frag");
    outlineSpriteShaderInit(&outlineShader);
    vec4s shaderColor = (vec4s){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};

    Texture t[2];
    textureLoad(t + 0, "assets/textures/player.png");
    textureLoad(t + 1, "assets/textures/playerMask.png");
    LineRenderer line;
    lineRendererCreate(&line, 5);
    Model quadModel, quadModel2;
    createQuadModel(&quadModel, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    createQuadModel(&quadModel2, 2.0f, 2.0f,
        -1.0f / (float)spriteScreen.width,
        1.0f + 1.0f / (float)spriteScreen.width,
        1.0f + 1.0f / (float)spriteScreen.height,
        -1.0f / (float)spriteScreen.height);

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
    camera.zoom = 1.0f/15.0f;
    camera.ang = 0.0f;
    cameraUpdateMatrix(&camera);

    line.segments[0].pos = (vec2s){ .x = 1.0f, .y = 10.0f };
    line.segments[0].uv1 = texturePixelToUV(t+0, 16, 14);
    line.segments[0].uv2 = texturePixelToUV(t+0, 18, 14);
    line.segments[0].size = 0.8f;
    line.segments[1].pos = (vec2s){ .x = 4.0f, .y = 8.0f };
    line.segments[1].uv1 = texturePixelToUV(t+0, 16, 16);
    line.segments[1].uv2 = texturePixelToUV(t+0, 18, 16);
    line.segments[1].size = 0.8f;
    line.segments[2].pos = (vec2s){ .x = 3.0f, .y = 0.0f };
    line.segments[2].uv1 = texturePixelToUV(t+0, 16, 23);
    line.segments[2].uv2 = texturePixelToUV(t+0, 18, 23);
    line.segments[2].size = 0.8f;
    line.segments[3].pos = (vec2s){ .x = 4.0f, .y = -10.0f };
    line.segments[3].uv1 = texturePixelToUV(t+0, 16, 30);
    line.segments[3].uv2 = texturePixelToUV(t+0, 18, 30);
    line.segments[3].size = 0.7f;
    line.segments[4].pos = (vec2s){ .x = 8.0f, .y = -10.0f };
    line.segments[4].uv1 = texturePixelToUV(t+0, 16, 32);
    line.segments[4].uv2 = texturePixelToUV(t+0, 18, 32);
    line.segments[4].size = 0.75f;

    int currentSegment = 0;
    bool lastLeft = false, lastRight = false, left = false, right = false;

    while (!glfwWindowShouldClose(window.internal)) {
        if (glfwGetKey(window.internal, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window.internal, GLFW_TRUE);

        if (glfwGetKey(window.internal, GLFW_KEY_W))
            camera.y += 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_S))
            camera.y -= 0.1f;

        if (glfwGetKey(window.internal, GLFW_KEY_U) && shaderColor.x < 1.0f) shaderColor.x += 0.02f;
        if (glfwGetKey(window.internal, GLFW_KEY_J) && shaderColor.x > 0.0f) shaderColor.x -= 0.02f;
        if (glfwGetKey(window.internal, GLFW_KEY_I) && shaderColor.y < 1.0f) shaderColor.y += 0.02f;
        if (glfwGetKey(window.internal, GLFW_KEY_K) && shaderColor.y > 0.0f) shaderColor.y -= 0.02f;
        if (glfwGetKey(window.internal, GLFW_KEY_O) && shaderColor.z < 1.0f) shaderColor.z += 0.02f;
        if (glfwGetKey(window.internal, GLFW_KEY_L) && shaderColor.z > 0.0f) shaderColor.z -= 0.02f;

        lastLeft = left; lastRight = right;
        left = glfwGetKey(window.internal, GLFW_KEY_COMMA);
        right = glfwGetKey(window.internal, GLFW_KEY_PERIOD);
        if (left && !lastLeft && currentSegment > 0) currentSegment--;
        if (right && !lastRight && currentSegment + 1 < line.numSegments) currentSegment++;

        if (glfwGetKey(window.internal, GLFW_KEY_LEFT)) line.segments[currentSegment].pos.x  -= 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_RIGHT)) line.segments[currentSegment].pos.x += 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_UP)) line.segments[currentSegment].pos.y    += 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_DOWN)) line.segments[currentSegment].pos.y  -= 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_LEFT_BRACKET)) line.segments[currentSegment].size   -= 0.05f;
        if (glfwGetKey(window.internal, GLFW_KEY_RIGHT_BRACKET)) line.segments[currentSegment].size  += 0.05f;
        lineRendererUpdateModel(&line);

        // Draw the legs without the outlines
        framebufferBind(&spriteScreen);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cameraUpdateMatrix(&spriteCamera);
        glUseProgram(shader.shader);
        glUniform4f(shader.mainColor, shaderColor.x, shaderColor.y, shaderColor.z, shaderColor.w);
        textureActivate(t+0, shader.texture0, 0);
        textureActivate(t+1, shader.texture1, 1);
        cameraUpdateShaderUniforms(&spriteCamera, shader.projection, shader.view);
        lineRendererRender(&line);

        // Apply the pixel outline to the legs
        framebufferBind(&outlineScreen);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(outlineShader.shader);
        glUniform2f(outlineShader.pixelStep, 1.0f / (float)spriteScreen.width, 1.0f / (float)spriteScreen.height);
        framebufferActivateColorTexture(&spriteScreen, outlineShader.texture0, 0);
        modelRender(&quadModel2, GL_TRIANGLES);

        // Draw the pixelated outlined legs to the normal world view
        windowUseDefaultFramebuffer(&window);
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cameraUpdateMatrix(&camera);
        glUseProgram(shader2.shader);
        glm_scale_make(modelMatrix, (vec3){8.0f, 8.0f, 1.0f});
        cameraUpdateShaderUniforms(&camera, shader2.projection, shader2.view);
        glUniformMatrix4fv(shader2.model, 1, GL_FALSE, (void *)modelMatrix);
        framebufferActivateColorTexture(&outlineScreen, shader2.texture0, 0);
        //textureActivate(t+0, shader2.texture0, 0);
        modelRender(&quadModel, GL_TRIANGLES);

        glfwSwapBuffers(window.internal);
        glfwPollEvents();
    }

    //modelFree(&m);
    framebufferDestroy(&spriteScreen);
    framebufferDestroy(&outlineScreen);
    modelFree(&quadModel);
    lineRendererDestroy(&line);
    glDeleteProgram(shader.shader);
    glDeleteProgram(shader2.shader);
    glDeleteProgram(outlineShader.shader);
    glDeleteTextures(1, &t[0].texID);
    glDeleteTextures(1, &t[1].texID);
    windowFree(&window);

    glfwTerminate();
    return 0;
}
