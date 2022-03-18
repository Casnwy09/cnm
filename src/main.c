#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/texture.h"
#include "gfx/camera.h"
#include "util/util.h"
#include "util/math.h"
#include "gfx/window.h"
#include "gfx/renderers/player.h"

static Camera camera;

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
    
    camera.x = 0.0f;
    camera.y = 0.0f;
    camera.zoom = 1.0f/15.0f;
    camera.ang = 0.0f;
    cameraUpdateMatrix(&camera);

    PlayerRenderer pr;
    playerRendererGlobalsInit();
    playerRendererCreate(&pr, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window.internal)) {
        if (glfwGetKey(window.internal, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window.internal, GLFW_TRUE);

        if (glfwGetKey(window.internal, GLFW_KEY_W))
            camera.y += 0.1f;
        if (glfwGetKey(window.internal, GLFW_KEY_S))
            camera.y -= 0.1f;

        // Update the legs
        vec2s mpos = cameraGetWorldCoordsFromMouse(&camera, window.internal);
        mpos.x /= 4.0f;
        mpos.y /= 4.0f;
        mpos = cameraGetWorldCoordsFromScreenSpace(playerRendererGetGlobalSpriteCam(), mpos.x, mpos.y);

        pr.ik.arms[1].ankle = (vec2s){ .x = pr.ik.arms[1].base.x - 4.55f, .y = pr.ik.arms[1].base.y - 7.7f };
        pr.ik.arms[0].ankle = (vec2s){ .x = pr.ik.arms[0].base.x + 10.0f, .y = pr.ik.arms[0].base.y };

        pr.ik.legs[0].ankle = (vec2s){ .x = pr.ik.legs[0].base.x + 3.94f, .y = pr.ik.legs[0].base.y - 8.64f };
        pr.ik.legs[1].ankle = (vec2s){ .x = pr.ik.legs[1].base.x - 4.20f, .y = pr.ik.legs[1].base.y - 9.70f };

        float mvspd = 0.01f;
        if (glfwGetKey(window.internal, GLFW_KEY_RIGHT)) pr.io.x += mvspd;
        if (glfwGetKey(window.internal, GLFW_KEY_LEFT))  pr.io.x -= mvspd;
        if (glfwGetKey(window.internal, GLFW_KEY_DOWN))  pr.io.y -= mvspd;
        if (glfwGetKey(window.internal, GLFW_KEY_UP))    pr.io.y += mvspd;

        if (glfwGetKey(window.internal, GLFW_KEY_SPACE)) {
            pr.ik.arms[1].ankle = mpos;
            pr.ik.arms[0].ankle = mpos;
        }

        float arm_angle = atan2f(pr.ik.arms[1].ankle.y - pr.ik.arms[1].base.y, pr.ik.arms[1].ankle.x - pr.ik.arms[1].base.x);
        pr.ik.arms[1].clockwise = (arm_angle > PI / 2.0f || arm_angle < -4.0f/5.0f * PI) ? true : false;

        if (glfwGetKey(window.internal, GLFW_KEY_ENTER) || glfwGetKey(window.internal, GLFW_KEY_RIGHT_SHIFT)) {
            float time = glfwGetTime() * 3.2f;
            if (glfwGetKey(window.internal, GLFW_KEY_RIGHT_SHIFT)) time *= 2.0f;
            float leg0Height = glm_clamp(sinf(time * PI), 0.0f, 1.0f)*4.0f;
            float leg1Height = glm_clamp(-sinf(time * PI), 0.0f, 1.0f)*4.0f;
            float leg0X = fabsf(fmodf(time, 2.0f)-1.0f)*-10.0f+1.5f;
            float leg1X = fabsf(fmodf(time, 2.0f)-1.0f)*10.0f-3.0f;
            float yShift = glm_clamp(sinf(time * PI), 0.0f, 1.0f) * 0.15f;

            pr.ik.legs[0].ankle.y += leg0Height - yShift*9.0f;
            pr.ik.legs[0].ankle.x += leg0X;
            pr.ik.legs[1].ankle.y += leg1Height - yShift*9.0f;
            pr.ik.legs[1].ankle.x += leg1X;

            pr.ik.arms[1].ankle.x += sinf(time * PI)*2.0f+3.0f;
            pr.ik.arms[1].ankle.y -= (cosf(2.0f*(time * PI) - (0.25f*PI/2.0f))+1.0f) / 2.0f;
            pr.ik.arms[0].ankle.y -= sinf(time * PI)*0.9f;

            pr.origin.x = sinf(time * 2.0f * PI) * 0.15f;
            pr.origin.y = yShift;
        }
        else {
            pr.origin.x = 0.0f;
            pr.origin.y = 0.0f;
        }

        playerRendererUpdate(&pr);

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        cameraUpdateMatrix(&camera);
        playerRendererRender(&pr, &window, &camera);

        glfwSwapBuffers(window.internal);
        glfwPollEvents();
    }

    playerRendererDestroy(&pr);
    playerRendererGlobalsFree();
    windowFree(&window);

    glfwTerminate();
    return 0;
}
