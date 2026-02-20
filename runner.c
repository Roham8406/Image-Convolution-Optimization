#include <unistd.h>
#include <time.h>
#include "convolution.c"
#include "optimizedConvolution.c"
#include "utils.c"

#include "dto.h"


int main(int argc, char argv[3][50]) {
    if (argc != 3) {
        printf("Usage: ./convolution.out [image input path] [convolution matrix path]\n");
        // return 1;
    }
    argc = 3;
    strcpy(argv[1], "Assets/PM.png");
    strcpy(argv[2], "Assets/matrix.txt");
    if (access(argv[1], F_OK)) {
        printf("File Not Found!\n");
        return 1;
    }
    FilesDTO data;
    switch (prepare(argv[1], argv[2], &data, 1)) {
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
    for (int i = 0; i < 384; i++) {
        data.in[ i]= i;
    }
    data.h = 4;
    data.w = 48;
    data.ch = 1;

    clock_t start1 = clock();
    convolve(data);
    clock_t end1 = clock();
    saveFile(argv[1], data, 0);
        
    clock_t start2 = clock();
    convolveOptimized(data);
    clock_t end2 = clock();
    saveFile(argv[1], data, 1);




    tidyup(&data);
    printf("Convolution took: \t\t\t %2.6fms\t\n", (double)(end1 - start1) / CLOCKS_PER_SEC * 1000);
    printf("Optiomized Convolution took: \t\t %2.6fms\n", (double)(end2 - start2) / CLOCKS_PER_SEC * 1000);
    printf("Optimized Version is faster by: \t %2.3f%%\n", (double) (end1 - start1) / (end2 - start2) * 100 - 100);
    return 0;
}
