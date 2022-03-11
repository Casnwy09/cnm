#include <stdio.h>
#include <stdlib.h>
#include "util.h"

char * fileToUTF8(const char * filePath) {
    FILE * fp;
    long len;
    char * str;

    if (!(fp = fopen(filePath, "rb"))) return NULL;
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    str = malloc(len + 1);
    fread(str, len, 1, fp);
    str[len] = '\0';
    fclose(fp);

    return str;
}

void generateModelMatrix(
    mat4 model, float x, float y, float z,
    float sx, float sy, float ang
) {
    glm_translate_make(model, (vec3){x, y, z});
    glm_rotate_z(model, ang, model);
    glm_scale(model, (vec3){sx, sy, 1.0});
}
