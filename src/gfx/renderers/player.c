#include "player.h"
#include "../../util/math.h"

typedef struct PlayerRendererGlobals {
    Texture color, mask;
    Framebuffer fbSprite, fbOutline;

    Camera camera;

    struct {
        Model fbModel;
        Model sprites;
        Model final;
        Model eyes;
    } models;

    struct {
        GenericShader outline, textured, line;
        SpriteShader sprites;
    } shaders;
} PlayerRendererGlobals;

PlayerRendererGlobals prg;

void playerRendererGlobalsInit(void) {
    // Create the framebuffers
    framebufferCreate(&prg.fbSprite, 32, 32, true);
    framebufferCreate(&prg.fbOutline, prg.fbSprite.width + 2, prg.fbSprite.height + 2, true);

    // Create the camera
    prg.camera.x = 0.0f;
    prg.camera.y = 0.0f;
    prg.camera.ang = 0.0f;
    prg.camera.zoom = 2.0f / (float)prg.fbSprite.width;
    cameraUpdateFromResize(&prg.camera, prg.fbSprite.width, prg.fbSprite.height);
    cameraUpdateMatrix(&prg.camera);

    // Create the shaders
    texturedShaderInit(&prg.shaders.textured, "assets/shaders/texturedShader/vertex.vert", NULL, "assets/shaders/texturedShader/fragment.frag");
    texturedShaderInit(&prg.shaders.line, "assets/shaders/playerShaders/line.vert", NULL, "assets/shaders/playerShaders/line.frag");
    outlineSpriteShaderInit(&prg.shaders.outline);
    spriteShaderInit(&prg.shaders.sprites, "assets/shaders/playerShaders/line.frag");

    // Create the textures
    textureLoad(&prg.color, "assets/textures/player.png");
    textureLoad(&prg.mask, "assets/textures/playerMask.png");

    // Create the models
    createQuadModel(&prg.models.final, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    createQuadModel(&prg.models.fbModel, 2.0f, 2.0f,
        -1.0f / (float)prg.fbSprite.width,
        1.0f + 1.0f / (float)prg.fbSprite.width,
        1.0f + 1.0f / (float)prg.fbSprite.height,
        -1.0f / (float)prg.fbSprite.height);
    modelInit(&prg.models.sprites, VT_SPRITE_ENTRY, false);
    modelInit(&prg.models.eyes, VT_SPRITE_ENTRY, false);
}
void playerRendererGlobalsFree(void) {
    framebufferDestroy(&prg.fbSprite);
    framebufferDestroy(&prg.fbOutline);
    modelFree(&prg.models.fbModel);
    modelFree(&prg.models.final);
    modelFree(&prg.models.sprites);
    glDeleteProgram(prg.shaders.line.shader);
    glDeleteProgram(prg.shaders.outline.shader);
    glDeleteProgram(prg.shaders.sprites.shader);
    glDeleteProgram(prg.shaders.textured.shader);
    glDeleteTextures(1, &prg.color.texID);
    glDeleteTextures(1, &prg.mask.texID);
}
static void initLegLine(LineRenderer *line, vec2s * base) {
    for (int i = 0; i < line->numSegments; i++) {
        float l = (float)i / ((float)line->numSegments - 1.0f);
        line->segments[i].pos = (vec2s){ .x = 5.0f, .y = -5.0f };
        line->segments[i].uv1 = texturePixelToUV(&prg.color, 16, (32.f-14.f)*l+14.f);
        line->segments[i].uv2 = texturePixelToUV(&prg.color, 18, (32.f-14.f)*l+14.f);
        line->segments[i].size = 0.6f;
    }

    *base = line->segments[0].pos;
}
static void initLegLine2(LineRenderer *line, vec2s * base) {
    initLegLine(line, base);
    line->segments[0].pos = (vec2s){ .x = -2.5f, .y = -5.0f };
    line->segments[0].size = 1.3f;
    line->segments[1].size = 1.0f;
    line->segments[2].size = 0.8f;
    line->segments[3].size = 1.1f;
    line->segments[4].size = 0.9f;
    *base = line->segments[0].pos;
}
static void initArmLine(LineRenderer *line, vec2s * base) {
    for (int i = 0; i < line->numSegments; i++) {
        float l = (float)i / ((float)line->numSegments - 1.0f);
        line->segments[i].pos = (vec2s){ .x = 5.5f, .y = 6.1f };
        line->segments[i].uv1 = texturePixelToUV(&prg.color, (32.f-18.f)*l+18.f, 30);
        line->segments[i].uv2 = texturePixelToUV(&prg.color, (32.f-18.f)*l+18.f, 32);
        line->segments[i].size = 0.5f;
    }

    *base = line->segments[0].pos;
}
static void initArmLine2(LineRenderer *line, vec2s * base) {
    initArmLine(line, base);
    line->segments[0].pos = (vec2s){ .x = -5.8f, .y = 6.7f };
    *base = line->segments[0].pos;
}
void playerRendererCreate(PlayerRenderer * pr, float ox, float oy) {
    lineRendererCreate(&pr->lines.legs[0], 5);
    lineRendererCreate(&pr->lines.legs[1], 5);
    lineRendererCreate(&pr->lines.arms[0], 5);
    lineRendererCreate(&pr->lines.arms[1], 5);
    limbIkCreate(&pr->ik.legs[0], pr->lines.legs[0].segments[0].pos, 5.0f, 4.75f, 1.6f, -0.5f*PI, true);
    limbIkCreate(&pr->ik.legs[1], pr->lines.legs[1].segments[0].pos, 5.0f, 5.0f, 1.6f, -0.5f*PI, true);
    limbIkCreate(&pr->ik.arms[0], pr->lines.arms[0].segments[0].pos, 5.5f, 4.5f, 0.0f, 0.0f, false);
    limbIkCreate(&pr->ik.arms[1], pr->lines.arms[1].segments[0].pos, 5.0f, 4.75f, 0.0f, -1.5f*PI, false);

    pr->origin.x = ox;
    pr->origin.y = oy;
    pr->io.x = 0.0f;
    pr->io.y = 0.0f;
    pr->color = (vec4s){{1.0f, 0.0f, 0.0f, 1.0f}};
    pr->eyeColor = (vec4s){{0.48f, 0.48f, 1.0f, 1.0f}};

    initLegLine(&pr->lines.legs[0], &pr->ik.legs[0].base);
    initLegLine2(&pr->lines.legs[1], &pr->ik.legs[1].base);
    initArmLine(&pr->lines.arms[0], &pr->ik.arms[0].base);
    initArmLine2(&pr->lines.arms[1], &pr->ik.arms[1].base);
}
void playerRendererDestroy(PlayerRenderer * pr) {
    lineRendererDestroy(&pr->lines.legs[0]);
    lineRendererDestroy(&pr->lines.legs[1]);
    lineRendererDestroy(&pr->lines.arms[0]);
    lineRendererDestroy(&pr->lines.arms[1]);
}
void playerRendererUpdate(PlayerRenderer * pr) {
#define SOLVE(state, line, bend)                                                    \
    limbIkSolve(&(state), (state).ankle);                                           \
    for (int i = 0; i < (int)(line).numSegments; i++)                               \
        line.segments[i].pos = limbIkSmooth(&(state), i, (line).numSegments, bend); \
    lineRendererUpdateModel(&(line))

    SOLVE(pr->ik.legs[0], pr->lines.legs[0], 3.0f);
    SOLVE(pr->ik.legs[1], pr->lines.legs[1], 3.0f);
    SOLVE(pr->ik.arms[0], pr->lines.arms[0], 2.0f);
    SOLVE(pr->ik.arms[1], pr->lines.arms[1], 2.0f);
}
static void drawPlayerLeg(PlayerRenderer * pr, LineRenderer * line) {
    alignas(16) mat4 trans;

    glUseProgram(prg.shaders.line.shader);
    glUniform4f(prg.shaders.line.mainColor, pr->color.x, pr->color.y, pr->color.z, pr->color.w);
    glm_translate_make(trans, (vec3){pr->io.x, pr->io.y, 0.0f});
    glUniformMatrix4fv(prg.shaders.line.model, 1, GL_FALSE, (void *)trans);
    textureActivate(&prg.color, prg.shaders.line.texture0, 0);
    textureActivate(&prg.mask, prg.shaders.line.texture1, 1);
    cameraUpdateShaderUniforms(&prg.camera, prg.shaders.line.projection, prg.shaders.line.view);
    lineRendererRender(line);
}
void playerRendererRender(PlayerRenderer * pr, Window * window, Camera * camera) {
    alignas(16) mat4 modelMatrix;

    // Create the sprites
    SpriteVertexEntry playerSprites[2];
    playerSprites[0].uvTopLeft = texturePixelToUV(&prg.color, 0.0f, 0.0f);
    playerSprites[0].uvBottomRight = texturePixelToUV(&prg.color, 12.0f, 12.0f);
    glm_translate_make(playerSprites[0].model.raw, (vec3){pr->io.x, pr->io.y, 0.0f});
    glm_scale(playerSprites[0].model.raw, (vec3){12.0f, 12.0f, 1.0f});

    playerSprites[1].uvTopLeft = texturePixelToUV(&prg.color, 16.0f, 0.0f);
    playerSprites[1].uvBottomRight = texturePixelToUV(&prg.color, 28.0f, 10.0f);
    glm_translate_make(playerSprites[1].model.raw, (vec3){pr->io.x, pr->io.y+11.0f, 0.0f});
    glm_scale(playerSprites[1].model.raw, (vec3){12.0f, 10.0f, 1.0f});

    modelBufferVerticies(&prg.models.sprites, sizeof(playerSprites), playerSprites, GL_DYNAMIC_DRAW);
    prg.models.sprites.numVerticies = 2;

    // Create the eye sprites
    SpriteVertexEntry eyes;
    eyes.uvTopLeft = texturePixelToUV(&prg.color, 0.0f, 16.0f);
    eyes.uvBottomRight = texturePixelToUV(&prg.color, 3.0f, 18.0f);
    glm_translate_make(eyes.model.raw, (vec3){pr->io.x+2.0f, pr->io.y+11.0f, 0.0f});
    glm_scale(eyes.model.raw, (vec3){3.0f, 2.0f, 1.0f});
    modelBufferVerticies(&prg.models.eyes, sizeof(eyes), &eyes, GL_DYNAMIC_DRAW);
    prg.models.eyes.numVerticies = 1;

    // Draw the legs without the outlines and the body
    framebufferBind(&prg.fbSprite);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    cameraUpdateMatrix(&prg.camera);

    // Draw the head and the body
    glUseProgram(prg.shaders.sprites.shader);
    glUniform4f(prg.shaders.sprites.mainColor, pr->color.x, pr->color.y, pr->color.z, pr->color.w);
    textureActivate(&prg.color, prg.shaders.sprites.texture0, 0);
    textureActivate(&prg.mask, prg.shaders.sprites.texture1, 1);
    cameraUpdateShaderUniforms(&prg.camera, prg.shaders.sprites.projection, prg.shaders.sprites.view);
    spriteModelRender(&prg.models.sprites);

    // Draw the eyes
    glUseProgram(prg.shaders.sprites.shader);
    glUniform4f(prg.shaders.sprites.mainColor, pr->eyeColor.x, pr->eyeColor.y, pr->eyeColor.z, pr->eyeColor.w);
    textureActivate(&prg.color, prg.shaders.sprites.texture0, 0);
    textureActivate(&prg.mask, prg.shaders.sprites.texture1, 1);
    cameraUpdateShaderUniforms(&prg.camera, prg.shaders.sprites.projection, prg.shaders.sprites.view);
    spriteModelRender(&prg.models.eyes);

    // Draw the legs and arms
    drawPlayerLeg(pr, &pr->lines.legs[0]);
    drawPlayerLeg(pr, &pr->lines.legs[1]);
    drawPlayerLeg(pr, &pr->lines.arms[0]);
    drawPlayerLeg(pr, &pr->lines.arms[1]);

    // Apply the pixel outline to the legs
    framebufferBind(&prg.fbOutline);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(prg.shaders.outline.shader);
    glUniform2f(prg.shaders.outline.pixelStep, 1.0f / (float)prg.fbSprite.width, 1.0f / (float)prg.fbSprite.height);
    framebufferActivateColorTexture(&prg.fbSprite, prg.shaders.outline.texture0, 0);
    modelRender(&prg.models.fbModel, GL_TRIANGLES);

    // Draw the pixelated outlined legs to the normal world view
    windowUseDefaultFramebuffer(window);
    glUseProgram(prg.shaders.textured.shader);
    cameraUpdateShaderUniforms(camera, prg.shaders.textured.projection, prg.shaders.textured.view);
    framebufferActivateColorTexture(&prg.fbOutline, prg.shaders.textured.texture0, 0);
    glm_translate_make(modelMatrix, (vec3){pr->origin.x, pr->origin.y, 0.0f});
    glm_scale(modelMatrix, (vec3){8.0f, 8.0f, 1.0f});
    glUniformMatrix4fv(prg.shaders.textured.model, 1, GL_FALSE, (void *)modelMatrix);
    modelRender(&prg.models.final, GL_TRIANGLES);
}
Camera * playerRendererGetGlobalSpriteCam(void) {
    return &prg.camera;
}
