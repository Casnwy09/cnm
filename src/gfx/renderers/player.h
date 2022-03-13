#ifndef _renderers_player_h_
#define _renderers_player_h_
#include "../framebuffer.h"
#include "../model.h"
#include "../shader.h"
#include "../texture.h"
#include "../camera.h"
#include "../window.h"
#include "../anim/limbIk.h"
#include "line.h"

typedef struct PlayerRenderer {
    struct {
        LineRenderer legs[2], arms[2];
    } lines;
    struct {
        LimbIKState legs[2], arms[2];
    } ik;

    vec2s origin;
    vec4s color, eyeColor;
} PlayerRenderer;

void playerRendererGlobalsInit(void);
void playerRendererGlobalsFree(void);
void playerRendererCreate(PlayerRenderer * pr, float ox, float oy);
void playerRendererDestroy(PlayerRenderer * pr);
void playerRendererUpdate(PlayerRenderer * pr);
void playerRendererRender(PlayerRenderer * pr, Window * window, Camera * camera);
Camera * playerRendererGetGlobalSpriteCam(void);

#endif
