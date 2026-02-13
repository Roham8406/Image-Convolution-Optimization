#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./convolution.out [image input path] [convolution matrix path]\n");
        return 1;
    }
    if (access(argv[1], F_OK)) {
        printf("File Not Found!\n");
        return 1;
    }
    FILE *matFile = fopen(argv[2], "r");

    if (!matFile) {
        printf("Matrix Not Found!\n");
        return 1;
    }

    int width, height, channels;
    char *input = stbi_load(argv[1], &width, &height, &channels, 0);
    if (!input) {
        printf("Failed to load image; Try changing the format!\n");
        return 1;
    }

    int matDeg, **mat;
    fscanf(matFile, "%d", &matDeg);
    mat = malloc(matDeg * matDeg * sizeof(int));
    for (int i = 0; i < matDeg*matDeg; i++){
        fscanf(matFile, "%d", mat + i);
    }
    fclose(matFile);
    
    char *output = malloc(width * height * channels);
    clock_t startTime = clock();
    // do the convolve
    clock_t endTime = clock();

    char* outputPath = malloc(strlen(argv[1]) + 20);
    sprintf(outputPath, "Convolved_%s", argv[1]);
    stbi_write_png(outputPath, width, height, channels, output, width * channels);

    stbi_image_free(input);
    free(output);
    free(outputPath);
    free(mat);

    printf("Convolution took %.6f seconds\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return 0;
}
