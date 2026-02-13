#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "convolution.c"


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

    fscanf(matFile, "%d", &data->deg);
    data->mat = malloc(data->deg * data->deg * sizeof(double));
    for (int i = 0; i < data->deg*data->deg; i++){
        fscanf(matFile, "%lf", data->mat + i);
    }
    fclose(matFile);
    
    data->out = malloc(data->w * data->h * data->ch);
    return 0;
}

void tidyup(char* image, FilesDTO data) {
    char* outputPath = malloc(strlen(image) + 20);
    sprintf(outputPath, "Convolved_%s", image);
    stbi_write_png(outputPath, data.w, data.h, data.ch, data.out, data.w * data.ch);

    stbi_image_free(data.in);
    free(data.out);
    free(outputPath);
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
    
    
    clock_t startTime = clock();
    convolve(data);
    clock_t endTime = clock();

    tidyup(argv[1], data);
    printf("Convolution took %.6f seconds\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return 0;
}
