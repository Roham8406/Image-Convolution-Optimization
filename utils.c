#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"
#include "dto.h"


void copyDTO(FilesDTO* a, FilesDTO* b) {                            //بازتولید دی‌تی‌او
    a->in = b->in;
    a->w = b->w;
    a->h = b->h;
    a->ch = b->ch;
    a->deg = b->deg;
}

int loadMat(const char* matrix, FilesDTO* data) {                   //بارگزاری مجدد ماتریس هسته
    if (data->mat) free(data->mat);                                 //آزادسازی حافظه در صورت نیاز
    FILE *matFile = fopen(matrix, "r");                             //باز کردن فایل

    if (!matFile) {
        return 1;
    }
    int x = fscanf(matFile, "%d", &data->deg);                      //اولین عدد نشانه مرتبهٔ ماتریس است
    int n = data->deg * data->deg;
    n = (int)((n + 15)/16) * 16;                                    //اطمینان از قابلیت جمع زدن درایه‌های ماتریس با ymm
    data->mat = (int*)calloc(n, sizeof(float));
    for (int i = 0; i < data->deg*data->deg; i++){
        x += fscanf(matFile, "%d", data->mat + i);                  //خواندن داده‌های ماتریس
    }
    fclose(matFile);                                                //بستن فایل
    return x;
}

int prepare(char *image, const char* matrix, FilesDTO* data, short forceGS) {       //آماده‌سازی دی‌تی‌او
    if (image) {
        if (forceGS) {
            data->in = stbi_load(image, &data->w, &data->h, &data->ch, 1);          //بارگذاری یک کاناله
            data->ch = 1;
        } else {
            data->in = stbi_load(image, &data->w, &data->h, &data->ch, 0);          //بارگذاری به هرگونه‌ای که نیاز است.
            if (data->ch == 3 || data->ch > 4) {
                data->in = stbi_load(image, &data->w, &data->h, &data->ch, 4);      //اگر ۳ کاناله بود، ۴ کاناله می‌کنیم تا ۸ بایت خواندن برای اس‌اس‌ای، یک پیکسل در دو دور متاوت بررسی نشود
                data->ch = 4;
            }
        }
        if (!data->in) {        
            return 2;
        }
    }

    

    int x = loadMat(matrix, data);                                                  //خواندن ماتریس
    
    data->out = (unsigned char*)calloc(data->w * data->h * data->ch + 200, 1);      //آماده‌سازی تصویر خروجی
    return x;
}

void saveFile(char *image, FilesDTO data, short isOpt) {                            //ذخیرهٔ تصویر خروجی
    char* outputPath = (char*) malloc(strlen(image) + 30);

    sprintf(outputPath, isOpt ? "Convolved/Opt_%s" : "Convolved/%s", image);
    stbi_write_png(outputPath, data.w, data.h, data.ch, data.out, data.w * data.ch);//ذخیرهٔ تصویر
    free(outputPath);
}


void tidyup(FilesDTO* data) {    //آزادسازی حافظه
    stbi_image_free(data->in);
    free(data->out);
    free(data->mat);
}