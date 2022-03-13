#include "../../util/math.h"
#include "limbIk.h"

void limbIkCreate(LimbIKState * state, vec2s base, float baseLen, float kneeLen, float ankleLen, float initialDir) {
    state->base = base;
    state->baseLen = baseLen;
    state->kneeLen = kneeLen;
    state->ankleLen = ankleLen;
    state->knee = (vec2s){ .x = state->knee.x + cosf(initialDir) * kneeLen, .y = state->knee.y + sinf(initialDir) * kneeLen };
    state->ankle = (vec2s){ .x = state->knee.x + cosf(initialDir) * kneeLen, .y = state->knee.y + sinf(initialDir) * kneeLen };
    state->foot = (vec2s){ .x = state->ankle.x + cosf(initialDir + 0.5f*PI) * ankleLen, .y = state->ankle.y + sinf(initialDir + 0.5f*PI) * ankleLen };
}
void limbIkSolve(LimbIKState * state, vec2s wantedPos) {
    float combinedLen = state->baseLen + state->kneeLen;

    // Solve the angle of the base and the knee
    float dist = glm_vec2_distance(state->base.raw, wantedPos.raw);
    if (dist >= combinedLen) {
        float ang = atan2f(wantedPos.y - state->base.y, wantedPos.x - state->base.x);
        state->knee = (vec2s){ .x = state->base.x + cosf(ang) * state->baseLen, .y = state->base.y + sinf(ang) * state->baseLen };
        state->ankle = (vec2s){ .x = state->knee.x + cosf(ang) * state->kneeLen, .y = state->knee.y + sinf(ang) * state->kneeLen };
    }
    else {
        // Guess we'll have to do this the hard way
        float kneeAng = acosf(
            ((dist*dist) - (state->baseLen*state->baseLen) - (state->kneeLen*state->kneeLen)) /
            (-2.0f * state->baseLen * state->kneeLen)
        );
        float baseAng = asinf((sinf(kneeAng)*state->kneeLen)/dist);
        kneeAng = -(PI - kneeAng - baseAng);

        float offset = atan2f(wantedPos.y - state->base.y, wantedPos.x - state->base.x);
        if (offset < 0.0f) offset = 2.0f*PI + offset;
        //offset = 2.0f*PI - offset;

        baseAng += offset;
        kneeAng += offset;

        state->knee = (vec2s){ .x = state->base.x + cosf(baseAng) * state->baseLen, .y = state->base.y + sinf(baseAng) * state->baseLen };
        state->ankle = (vec2s){ .x = state->knee.x + cosf(kneeAng) * state->kneeLen, .y = state->knee.y + sinf(kneeAng) * state->kneeLen };
    }

    // Solve the foot angle
    float footAng = atan2f(state->ankle.y - state->knee.y, state->ankle.x - state->knee.x) + 0.5f*PI;
    state->foot = (vec2s){ .x = state->ankle.x + cosf(footAng) * state->ankleLen, .y = state->ankle.y + sinf(footAng) * state->ankleLen };
}

vec2s limbIkSmooth(LimbIKState * state, int index, int numIndicies, float bend) {
    if (index == 0) return state->base;
    if (index == numIndicies / 2) return state->knee;
    if (index == numIndicies - 1) return state->ankle;

    float dist = glm_vec2_distance(state->ankle.raw, state->base.raw);
    float legBend = ((dist*dist) - (state->baseLen*state->baseLen) - (state->kneeLen*state->kneeLen)) /
        (-2.0f * state->baseLen * state->kneeLen);
    legBend = (legBend + 1.0f) / 2.0f;

    if (index < numIndicies / 2) {
        float t = (float)index / (float)(numIndicies / 2);
        vec2s perp;
        glm_vec2_sub(state->knee.raw, state->base.raw, perp.raw);
        glm_vec2_normalize(perp.raw);
        glm_vec2_rotate(perp.raw, PI*0.5f, perp.raw);
        glm_vec2_scale(perp.raw, t * bend * legBend, perp.raw);

        vec2s result;
        glm_vec2_lerp(state->base.raw, state->knee.raw, t, result.raw);
        glm_vec2_add(result.raw, perp.raw, result.raw);
        return result;
    }
    else {
        float t = (float)(index - numIndicies / 2) / (float)(numIndicies / 2);
        vec2s perp;
        glm_vec2_sub(state->ankle.raw, state->knee.raw, perp.raw);
        glm_vec2_normalize(perp.raw);
        glm_vec2_rotate(perp.raw, PI*0.5f, perp.raw);
        glm_vec2_scale(perp.raw, t * bend * legBend, perp.raw);

        vec2s result;
        glm_vec2_lerp(state->knee.raw, state->ankle.raw, t, result.raw);
        glm_vec2_add(result.raw, perp.raw, result.raw);
        return result;
    }
}
