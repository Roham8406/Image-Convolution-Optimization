#include <unistd.h>
#include <time.h>
#include "convolution.c"
#include "optimizedConvolution.c"
#include "utils.c"

#include "dto.h"


int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./convolution.out [image input path] [convolution matrix path]\n");
        return 1;
    }
    FilesDTO data;
    switch (prepare(argv[1], argv[2], &data, 0)) {                          //آماده سازی برای کانوالو
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

    clock_t start1 = clock();
    convolve(data);                                                         //کانوالو غیر بهینه
    clock_t end1 = clock();
    saveFile(argv[1], data, 0);                                             // ذخیرهٔ فایل کانوالو شده
        
    clock_t start2 = clock();
    convolveOptimized(data);                                                //گانوالو بهینه
    clock_t end2 = clock();
    saveFile(argv[1], data, 1);                                             //ذخیرهٔ فایل کانوالو شده




    tidyup(&data);
    printf("Convolution took: \t\t\t %2.6fms\t\n", (double)(end1 - start1) / CLOCKS_PER_SEC * 1000);
    printf("Optiomized Convolution took: \t\t %2.6fms\n", (double)(end2 - start2) / CLOCKS_PER_SEC * 1000);
    printf("Optimized Version is faster by: \t %2.3f%%\n", (double) (end1 - start1) / (end2 - start2) * 100 - 100);
    return 0;
}
