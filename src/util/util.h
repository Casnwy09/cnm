#ifndef _util_h_
#define _util_h_
#include <cglm/cglm.h>

char * fileToUTF8(const char * filePath);
void generateModelMatrix(
    mat4 model, float x, float y, float z,
    float sx, float sy, float ang
);

#endif
