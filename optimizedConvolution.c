#include "dto.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int sum_matrix(int* mat, int size);
extern void convolve_asm(int* mat, unsigned char* in, unsigned char* out, 
                             int w, int ch, int deg, int sum, int centre, int i, int j);

#ifdef __cplusplus
}
#endif

void convolveOptimized(FilesDTO input) {
    if (input.ch != 4 && input.ch != 1) {
        // return;
    }
    int sum = sum_matrix(input.mat, input.deg*input.deg);                   //جمع درایه‌های ماتریس
    int centre = input.deg/2;                                               //مرکز ماتریس هسته
    for (int i = centre; i < input.w - centre + 7; i+=8/input.ch) {         //بررسی 8 باید در هر دور
        for (int j = centre; j < input.h - centre; j++) {
            convolve_asm(input.mat, input.in, input.out,
                         input.w, input.ch, input.deg, sum, centre, i, j);  //کانوالو 8 بایت بدست آمده
        }
    }
}