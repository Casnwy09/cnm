#include <stdio.h>
#include <stdlib.h>

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
