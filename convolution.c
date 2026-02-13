#include "dto.h"

void convolve(FilesDTO input) {
    float sum = 0;
    for (int i = input.deg*input.deg-1; i >= 0 ; i--) {
        sum += input.mat[i];
    }
    sum = sum > 0.0001 ? sum : 1;
    int centre = input.deg/2;
    for (int i = 0; i < input.w; i++) {
        for (int j = 0; j < input.h; j++) {
            for (int k = 0; k < input.ch; k++) {
                double cSum = 0.0;
                for (int l = -centre; l <= centre; l++) {
                    for (int m = -centre; m <= centre; m++) {
                        int i1 = i + l, j1 = j + m;
                        i1 = i1 >= input.w ? i1-input.w : i1 < 0 ? i1 + input.w : i1;
                        j1 = j1 >= input.h ? j1-input.h : j1 < 0 ? j1 + input.h : j1;
                        cSum += input.in[(j1 * input.w + i1) * input.ch + k] * input.mat[(m + centre) * input.deg  + l + centre];
                    }
                }

                cSum /= sum;
                cSum = cSum > 255 ? 255 : cSum < 0 ? 0 : cSum;
                input.out[(j*input.w + i)*input.ch+ k] = (unsigned char)cSum;
            }
        }
    }
}