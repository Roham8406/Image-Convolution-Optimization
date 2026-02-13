#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"


void convolve(unsigned char *in, unsigned char *out, int w,
     int h, int ch, double *mat, int deg
) {
    int sum = 0;
    for (int i = deg*deg-1; i >= 0 ; i--) {
        sum += mat[i];
    }
    sum = sum > 0.0001 ? sum : 1;
    int centre = deg/2;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < ch; k++) {
                double cSum = 0.0;
                for (int l = -centre; l <= centre; l++) {
                    for (int m = -centre; m <= centre; m++) {
                        int i1 = i + l, j1 = j + m;
                        i1 = i1 >= w ? i1-w : i1 < 0 ? i1 + w : i1;
                        j1 = j1 >= h ? j1-h : j1 < 0 ? j1 + h : j1;
                        cSum += in[(j1 * w + i1) * ch + k] * mat[(m + centre) * deg  + l + centre];
                    }
                }

                cSum /= sum;
                cSum = cSum > 255 ? 255 : cSum < 0 ? 0 : cSum;
                out[(j*w + i)*ch+ k] = (unsigned char)cSum;
            }
        }
    }
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
    FILE *matFile = fopen(argv[2], "r");

    if (!matFile) {
        printf("Matrix Not Found!\n");
        return 1;
    }

    int width, height, channels;
    unsigned char *input = stbi_load(argv[1], &width, &height, &channels, 0);
    if (!input) {
        printf("Failed to load image; Try changing the format!\n");
        return 1;
    }

    int matDeg;
    double *mat;
    fscanf(matFile, "%d", &matDeg);
    mat = malloc(matDeg * matDeg * sizeof(double));
    for (int i = 0; i < matDeg*matDeg; i++){
        fscanf(matFile, "%lf", mat + i);
    }
    fclose(matFile);
    
    unsigned char *output = malloc(width * height * channels);


    clock_t startTime = clock();
    convolve(input, output, width, height, channels, mat, matDeg);
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
