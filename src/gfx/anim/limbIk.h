#ifndef _limbIk_h_
#define _limbIk_h_
#include <cglm/types-struct.h>

typedef struct LimbIKState {
    vec2s base, knee;
    vec2s ankle, foot;

    bool clockwise;
    float baseLen, kneeLen, ankleLen;
} LimbIKState;

void limbIkCreate(LimbIKState * state, vec2s base, float baseLen, float kneeLen, float ankleLen, float initialDir, bool bendClockwise);
void limbIkSolve(LimbIKState * state, vec2s wantedPos);
vec2s limbIkSmooth(LimbIKState * state, int index, int numIndicies, float bend);

#endif
