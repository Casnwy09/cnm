#include "math.h"

vec2s lineIntersection(vec2s l1a, vec2s l1b, vec2s l2a, vec2s l2b) {
    float a1 = l1b.y - l1a.y;
    float b1 = l1a.x - l1b.x;
    float c1 = a1*(l1a.x) + b1*(l1a.y);
  
    float a2 = l2b.y - l2a.y;
    float b2 = l2a.x - l2b.x;
    float c2 = a2*(l2a.x)+ b2*(l2a.y);
  
    float det = a1*b2 - a2*b1;
  
    if (det == 0)   // Lines are parrallel
    {
        return (vec2s){{FLT_MAX, FLT_MAX}};
    }
    else
    {
        float x = (b2*c1 - b1*c2)/det;
        float y = (a1*c2 - a2*c1)/det;
        return (vec2s){{x, y}};
    }
}
