#include "dto.h"

extern int sum_matrix(int* mat, int size);
extern void convolve_asm(int* mat, unsigned char* in, unsigned char* out, 
                             int w, int ch, int deg, int sum, int centre, int i, int j);

void convolveOptimized(FilesDTO input) {
    if (input.ch != 4) {
        // return;
    }
    int sum = sum_matrix(input.mat, input.deg*input.deg);
    int centre = input.deg/2;
    for (int i = centre; i < input.w - 1 - centre; i+=2) {
        for (int j = centre; j < input.h - centre; j++) {
            convolve_asm(input.mat, input.in, input.out, 
                         input.w, input.ch, input.deg, sum, centre, i, j);
        }
    }
}