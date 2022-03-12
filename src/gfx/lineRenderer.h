#ifndef _lineRenderer_h_
#define _lineRenderer_h_
#include "model.h"

typedef struct LineRendererSegment {
    float size;
    vec2s pos;
    vec4s color;
} LineRendererSegment;

typedef struct LineRenderer {
    Model model;

    float z;
    size_t segmentsCapacity;
    size_t numSegments;
    LineRendererSegment * segments;
    Vertex * verticies;
} LineRenderer;

void lineRendererCreate(LineRenderer * lr, size_t numSegments);
void lineRendererDestroy(LineRenderer * lr);
void lineRendererUpdateModel(LineRenderer * lr);
void lineRendererRender(LineRenderer * lr);
void lineRendererResize(LineRenderer * lr, size_t newNumSegments);

#endif
