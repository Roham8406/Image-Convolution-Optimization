#include "dto.h"

extern int sum_matrix(int* mat, int size);
extern void convolve_asm(int cell, unsigned char* image, int channels, int xOffset, int yOffset);
extern void store_convolve_result(unsigned char* out, int ch, int sum);

void convolveOptimized(FilesDTO input) {
    if (input.ch != 4) {
        return;
    }
    int sum = sum_matrix(input.mat, input.deg*input.deg);
    int centre = input.deg/2;
    for (int i = centre; i < input.w - 7 - centre; i++) {
        for (int j = centre; j < input.h - centre; j++) {
            asm volatile (
                "vxorps %ymm0, %ymm0, %ymm0\n\t"
                "vxorps %ymm1, %ymm1, %ymm1\n\t"
                "vxorps %ymm2, %ymm2, %ymm2\n\t"
                "vxorps %ymm3, %ymm3, %ymm3\n\t"
            );
            for (int ki = -centre; ki <= centre; ki++) {
                for (int kj = -centre; kj <= centre; kj++) {
                    convolve_asm(input.mat[(centre+ki)*input.deg + (centre+kj)], input.in + ((j + kj) * input.w + i + ki)* input.ch, input.ch, kj, ki);
                }
            }
            store_convolve_result(input.out + (j*input.w + i) * input.ch, input.ch, sum);
        }
    }
}