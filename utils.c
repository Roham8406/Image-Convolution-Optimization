#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"
#include "dto.h"

int loadMat(char* matrix, FilesDTO* data) {
    if (data->mat) free(data->mat);
    FILE *matFile = fopen(matrix, "r");

    if (!matFile) {
        
        return 1;
    }
    int x = fscanf(matFile, "%d", &data->deg);
    int n = data->deg * data->deg;
    n = (int)((n + 15)/16) * 16;                                  //Making sure it can adapt with ymm for summing
    data->mat = calloc(n, sizeof(float));
    for (int i = 0; i < data->deg*data->deg; i++){
        x += fscanf(matFile, "%d", data->mat + i);
    }
    fclose(matFile);
    return x;
}

int prepare(char *image, char* matrix, FilesDTO* data, short forceGS) {
    if (forceGS) {
        data->in = stbi_load(image, &data->w, &data->h, &data->ch, 1);
        data->ch = 1;
    } else {
        data->in = stbi_load(image, &data->w, &data->h, &data->ch, 0);
        if (data->ch == 3 || data->ch > 4) {
            data->in = stbi_load(image, &data->w, &data->h, &data->ch, 4);
            data->ch = 4;
        }
    }

    if (!data->in) {        
        return 2;
    }

    int x = loadMat(matrix, data);
    
    data->out = calloc(data->w * data->h * data->ch + 200, 1);
    return x == 0;
}

void saveFile(char *image, FilesDTO data, short isOpt) {
    char* outputPath = malloc(strlen(image) + 30);
    sprintf(outputPath, "build/debug%d.bin", isOpt);
    FILE *f = fopen(outputPath, "wb");
    fwrite(data.out, sizeof(unsigned char), data.w * data.h * data.ch, f);
    fclose(f);


    sprintf(outputPath, isOpt ? "Convolved/Opt_%s" : "Convolved/%s", image);
    stbi_write_png(outputPath, data.w, data.h, data.ch, data.out, data.w * data.ch);
    free(outputPath);
}


void tidyup(FilesDTO* data) {
    stbi_image_free(data->in);
    free(data->out);
    free(data->mat);
}