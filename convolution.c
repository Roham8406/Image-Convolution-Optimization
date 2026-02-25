#include "dto.h"

void convolve(FilesDTO input) {
    int sum = 0;
    for (int i = input.deg*input.deg-1; i >= 0 ; i--) {
        sum += input.mat[i];                                                            //محاسبهٔ جمع درایه‌های ماتریس
    }
    sum = sum > 0 ? sum : 1;
    int centre = input.deg/2;
    for (int i = centre; i < input.w - centre; i++) {
        for (int j = centre; j < input.h - centre; j++) {
            for (int k = 0; k < input.ch; k++) {
                int cSum = 0;
                for (int l = -centre; l <= centre; l++) {
                    for (int m = -centre; m <= centre; m++) {
                        int i1 = i + l, j1 = j + m;
                        cSum += input.in[(j1 * input.w + i1) * input.ch + k] * 
                                input.mat[(m + centre) * input.deg  + l + centre];      //محاسبهٔ ضرب درایهٔ ماتریس
                    }
                }

                cSum /= sum;                                                            //هنجار کردن درایهٔ ماتریس
                cSum = cSum > 255 ? 255 : cSum < 0 ? 0 : cSum;
                input.out[(j*input.w + i)*input.ch+ k] = (unsigned char)cSum;           //ذخیرهٔ درایهٔ ماتریس
            }
        if (input.ch != 1 && sum<=0) 
            input.out[(j*input.w + i)*input.ch+ input.ch - 1] = 
                input.in[(j*input.w + i)*input.ch+ input.ch - 1];                       //جلوگیری از بی‌رنگی‌ توسط 0 شدن کانال آلفا
        }
    }
}