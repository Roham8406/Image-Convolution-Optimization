#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "optimizedConvolution.c"
#include "utils.c"

extern long long judge(unsigned char* out, unsigned char threshold, long long size);

double calc(double hori, double nw, double ne, double n, double vert, double edge, long long size) {
    double curves = (nw + ne + n);

    double score = 4 * hori + curves + 2 * edge - 5 * vert;
    return score / size;
}

void checkImage(char *image, int isVicotrian) {
    FilesDTO data = {0};
    prepare(image, "Assets/TowerCheck/Blur.txt", &data, 1);
    convolveOptimized(data);
    memmove(data.in, data.out, data.w * data.h * data.ch);
    long long size = ((long long) (data.w)) * data.h * data.ch;

    loadMat("Assets/TowerCheck/Hori.txt", &data);
    clock_t sum1 = 0, sum2 = 0;
    sum1 -= clock();
    convolveOptimized(data);
    long long c1 = judge(data.out, 145, size);
    sum1 += clock();
    // saveFile(image, data, 1); //Optimzed ~~ Vert


    loadMat("Assets/TowerCheck/Curve1.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c2 = judge(data.out, 135, size);
    sum1 += clock();

    loadMat("Assets/TowerCheck/Curve2.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c3 = judge(data.out, 135, size);
    sum1 += clock();

    loadMat("Assets/TowerCheck/Curve3.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c4 = judge(data.out, 135, size);
    sum1 += clock();
    // saveFile(image, data, 0); //Not Optimzed ~~ Hori
    
    loadMat("Assets/TowerCheck/Vert.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c5 = judge(data.out, 127, size);
    sum1 += clock();

    loadMat("Assets/TowerCheck/Edge.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c6 = judge(data.out, 50, size);
    sum1 += clock();

    double score = calc(c1,c2,c3,c4,c5,c6, size);

    long long sum = data.w * data.h;


    tidyup(&data);
    printf("*** %s ***\n", image);
    printf("Progress took: \t\t\t %2.6fms\n", (double) sum1 / CLOCKS_PER_SEC * 1000);
    printf("Victorian sharp edges count: \t %lld \t %lld\n", c5, c6);
    printf("Iraninan sharp edges count: \t %lld \t %lld \t %lld \t %lld\n", c1, c2, c3, c4);
    printf("Score: %f\n", score);
    printf(score < 0.25 ? "The Architecture is Victorian." : "The Architecture is Iraninan.");
    printf("\n****************************************\n\n\n");
}

int main() {
    char path[50];
    for (int i = 1; i < 16; i++) {
        sprintf(path, "Assets/TowerCheck/%d.jpg", i);
        checkImage(path, i > 8);
    }

    return 0;
}
