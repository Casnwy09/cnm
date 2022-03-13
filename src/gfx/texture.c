#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "texture.h"

void textureLoad(Texture * tex, const char * file) {
    int width, height, channels;
    uint8_t * imgData;

    glGenTextures(1, &tex->texID);
    glBindTexture(GL_TEXTURE_2D, tex->texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    if (!(imgData = stbi_load(file, &width, &height, &channels, 0))) {
        printf("textureLoad: Can't load the texture \"%s\"!\n", file);
    }
    else {
        int imgFormat = (channels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, imgData);
    }
    stbi_image_free(imgData);
    tex->width = width;
    tex->height = height;
}
void textureActivate(Texture * tex, UniformLoc sampler2D, int textureSlot) {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, tex->texID);
    glUniform1i(sampler2D, textureSlot);
}
vec2s texturePixelToUV(Texture * tex, float px, float py) {
    return (vec2s){.x = px / (float)tex->width, .y = 1.0f - py / (float)tex->height};
}
