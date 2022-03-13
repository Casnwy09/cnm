#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "../util/math.h"
#include "lineRenderer.h"

#define SEGMENTS_INC 8

void lineRendererCreate(LineRenderer * lr, size_t numSegments) {
    memset(lr, 0, sizeof(*lr));
    modelInit(&lr->model, VT_COLORED, false);
    lineRendererResize(lr, numSegments);
}
void lineRendererDestroy(LineRenderer * lr) {
    modelFree(&lr->model);
    free(lr->segments);
    free(lr->verticies);
}
void lineRendererUpdateModel(LineRenderer * lr) {
    vec2s lineTemp;
    vec2s linePerp[2];
    vec2s finalPt;
    vec2s aa, ab, ba, bb;
    size_t i;

    // Get normalized vector in the direction of the first line segment and create the first 2 verticices
    lineTemp.x = lr->segments[1].pos.x - lr->segments[0].pos.x;
    lineTemp.y = lr->segments[1].pos.y - lr->segments[0].pos.y;
    glm_vec2_normalize(lineTemp.raw);
    linePerp[1].x = lineTemp.y;
    linePerp[1].y = -lineTemp.x;

    lr->verticies[0].pos.x = lr->segments[0].pos.x + linePerp[1].x * lr->segments[0].size;
    lr->verticies[0].pos.y = lr->segments[0].pos.y + linePerp[1].y * lr->segments[0].size;
    lr->verticies[0].pos.z = lr->z;
    lr->verticies[0].color = lr->segments[0].color;
    lr->verticies[0].uv = lr->segments[0].uv1;
    lr->verticies[1].pos.x = lr->segments[0].pos.x - linePerp[1].x * lr->segments[0].size;
    lr->verticies[1].pos.y = lr->segments[0].pos.y - linePerp[1].y * lr->segments[0].size;
    lr->verticies[1].pos.z = lr->z;
    lr->verticies[1].color = lr->segments[0].color;
    lr->verticies[1].uv = lr->segments[0].uv2;

    for (i = 1; i < lr->numSegments - 1; i++) {
        linePerp[0] = linePerp[1];
        lineTemp.x = lr->segments[i+1].pos.x - lr->segments[i].pos.x;
        lineTemp.y = lr->segments[i+1].pos.y - lr->segments[i].pos.y;
        glm_vec2_normalize(lineTemp.raw);
        linePerp[1].x = lineTemp.y;
        linePerp[1].y = -lineTemp.x;

        for (int j = 0; j < 2; j++) {
            aa.x = lr->segments[i-1].pos.x + linePerp[0].x * lr->segments[i-1].size;
            aa.y = lr->segments[i-1].pos.y + linePerp[0].y * lr->segments[i-1].size;

            ab.x = lr->segments[i].pos.x + linePerp[0].x * lr->segments[i].size;
            ab.y = lr->segments[i].pos.y + linePerp[0].y * lr->segments[i].size;

            ba.x = lr->segments[i+1].pos.x + linePerp[1].x * lr->segments[i+1].size;
            ba.y = lr->segments[i+1].pos.y + linePerp[1].y * lr->segments[i+1].size;

            bb.x = lr->segments[i].pos.x + linePerp[1].x * lr->segments[i].size;
            bb.y = lr->segments[i].pos.y + linePerp[1].y * lr->segments[i].size;
            finalPt = lineIntersection(aa, ab, ba, bb);

            if (finalPt.x == FLT_MAX || finalPt.y == FLT_MAX || 
                lineIntersectionDet(aa, ab, ba, bb) < 1.0f) {
                finalPt.x = lr->segments[i].pos.x + linePerp[1].x * lr->segments[i].size;
                finalPt.y = lr->segments[i].pos.y + linePerp[1].y * lr->segments[i].size;
            }

            lr->verticies[i*2+j].pos.x = finalPt.x;
            lr->verticies[i*2+j].pos.y = finalPt.y;
            lr->verticies[i*2+j].pos.z = lr->z;
            lr->verticies[i*2+j].color = lr->segments[i].color;
            lr->verticies[i*2+j].uv = j ? lr->segments[i].uv2 : lr->segments[i].uv1;
            glm_vec2_negate(linePerp[0].raw);
            glm_vec2_negate(linePerp[1].raw);
        }
    }

    lineTemp.x = lr->segments[i].pos.x - lr->segments[i-1].pos.x;
    lineTemp.y = lr->segments[i].pos.y - lr->segments[i-1].pos.y;
    glm_vec2_normalize(lineTemp.raw);
    linePerp[1].x = lineTemp.y;
    linePerp[1].y = -lineTemp.x;

    lr->verticies[i*2+0].pos.x = lr->segments[i].pos.x + linePerp[1].x * lr->segments[i].size;
    lr->verticies[i*2+0].pos.y = lr->segments[i].pos.y + linePerp[1].y * lr->segments[i].size;
    lr->verticies[i*2+0].pos.z = lr->z;
    lr->verticies[i*2+0].color = lr->segments[i].color;
    lr->verticies[i*2+0].uv = lr->segments[i].uv1;
    lr->verticies[i*2+1].pos.x = lr->segments[i].pos.x - linePerp[1].x * lr->segments[i].size;
    lr->verticies[i*2+1].pos.y = lr->segments[i].pos.y - linePerp[1].y * lr->segments[i].size;
    lr->verticies[i*2+1].pos.z = lr->z;
    lr->verticies[i*2+1].color = lr->segments[i].color;
    lr->verticies[i*2+1].uv = lr->segments[i].uv2;

    lr->model.numVerticies = lr->numSegments * 2;
    modelBufferVerticies(&lr->model, sizeof(ColoredVertex) * lr->model.numVerticies, lr->verticies, GL_DYNAMIC_DRAW);
}
void lineRendererRender(LineRenderer * lr) {
    modelRenderVerticies(&lr->model, GL_TRIANGLE_STRIP);
}
void lineRendererResize(LineRenderer * lr, size_t newNumSegments) {
    size_t oldSegmentsCapacity = lr->segmentsCapacity;

    lr->numSegments = newNumSegments;
    if (lr->numSegments > lr->segmentsCapacity) {
        lr->segmentsCapacity = (lr->numSegments / SEGMENTS_INC + 1) * SEGMENTS_INC;
        lr->segments = realloc(lr->segments, sizeof(LineRendererSegment) * lr->segmentsCapacity);
        lr->verticies = realloc(lr->verticies, sizeof(ColoredVertex) * 2 * lr->segmentsCapacity);

        for (size_t i = oldSegmentsCapacity; i < lr->segmentsCapacity; i++) {
            lr->segments[i].color = (vec4s){.w = 1.0f, .x = 1.0f, .y = 1.0f, .z = 1.0f};
            lr->segments[i].size = 1.0f;
            lr->segments[i].pos = (vec2s){.x = 0.0f, .y = 0.0f};
            lr->segments[i].uv1 = (vec2s){.x = 0.0f, .y = 1.0f};
            lr->segments[i].uv2 = (vec2s){.x = 1.0f, .y = 1.0f};
        }

        for (size_t i = oldSegmentsCapacity; i < lr->segmentsCapacity * 2; i++) {
            lr->verticies[i].uv = (vec2s){.x = 0.0f, .y = 0.0f};
        }
    }
}
