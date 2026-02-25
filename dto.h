#ifndef FILES_DTO
#define FILES_DTO

typedef struct {
    unsigned char* in;      //تصویر ورودی
    unsigned char* out;     //تصویر خروجی
    int w;                  //عرض تصویر
    int h;                  //ارتفاع تصویر
    int ch;                 //تعداد کانال‌های تصویر
    int* mat;               //ماتریس هسته
    int deg;                //درجهٔ ماتریس هسته
} FilesDTO;

#endif
