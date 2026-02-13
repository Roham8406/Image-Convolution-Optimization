#ifndef FILES_DTO
#define FILES_DTO

typedef struct {
    unsigned char* in;      //Input image
    unsigned char* out;     //Output image
    int w;                  //Image Width
    int h;                  //Image Height
    int ch;                 //Image Channels
    float* mat;            //Kernel Matrix
    int deg;                //Kernel Degree
} FilesDTO;

#endif
