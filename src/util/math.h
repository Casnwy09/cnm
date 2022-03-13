#ifndef _math_h_
#define _math_h_
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

#define PI 3.14195265f

vec2s lineIntersection(vec2s l1a, vec2s l1b, vec2s l2a, vec2s l2b);
float lineIntersectionDet(vec2s l1a, vec2s l1b, vec2s l2a, vec2s l2b);

#endif
