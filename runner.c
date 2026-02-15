#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "convolution.c"
#include "optimizedConvolution.c"


#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"
#include "dto.h"

int prepare(char *image, char* matrix, FilesDTO* data) {
    FILE *matFile = fopen(matrix, "r");

    if (!matFile) {
        
        return 1;
    }

    data->in = stbi_load(image, &data->w, &data->h, &data->ch, 0);
    if (!data->in) {
        
        return 2;
    }

    int x = fscanf(matFile, "%d", &data->deg);
    int n = data->deg * data->deg;
    n = (int)((n + 15)/16) * 16;                                  //Making sure it can adapt with ymm for summing
    data->mat = calloc(n, sizeof(float));
    for (int i = 0; i < data->deg*data->deg; i++){
        x += fscanf(matFile, "%d", data->mat + i);
    }
    fclose(matFile);
    
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

void tidyup(FilesDTO data) {
    stbi_image_free(data.in);
    free(data.out);
    free(data.mat);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./convolution.out [image input path] [convolution matrix path]\n");
        return 1;
    }
    if (access(argv[1], F_OK)) {
        printf("File Not Found!\n");
        return 1;
    }
    FilesDTO data;
    switch (prepare(argv[1], argv[2], &data)) {
        case 0: break;
        case 1: {
            printf("Matrix Not Found!\n");
            return 1;
        }
            break;
        case 2: {
            printf("Failed to load image; Try changing the format!\n");
            return 1;
        }

    }
    
    clock_t start2 = clock();
    convolveOptimized(data);
    clock_t end2 = clock();
    saveFile(argv[1], data, 1);
    
    clock_t start1 = clock();
    convolve(data);
    clock_t end1 = clock();
    saveFile(argv[1], data, 0);


    tidyup(data);
    printf("Convolution took: \t\t\t %2.6fms\t\n", (double)(end1 - start1) / CLOCKS_PER_SEC * 1000);
    printf("Optiomized Convolution took: \t\t %2.6fms\n", (double)(end2 - start2) / CLOCKS_PER_SEC * 1000);
    printf("Optimized Version is faster by: \t %2.3f%%\n", (double) (end1 - start1) / (end2 - start2) * 100 - 100);
    return 0;
}
