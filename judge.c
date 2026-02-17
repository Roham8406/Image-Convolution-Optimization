#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "optimizedConvolution.c"
#include "utils.c"
#include "judgeThreshold.c"

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

    loadMat("Assets/TowerCheck/Hori.txt", &data);
    clock_t sum1 = 0, sum2 = 0;
    sum1 -= clock();
    convolveOptimized(data);
    long long c1 = threshold(data.out, 145, ((long long) (data.w)) * data.h * data.ch);
    sum1 += clock();
    saveFile(image, data, 1); //Optimzed ~~ Vert


    loadMat("Assets/TowerCheck/Curve1.txt", &data);
    sum1 -= clock();
    convolveOptimized(data);
    long long c2 = threshold(data.out, 135, ((long long) (data.w)) * data.h * data.ch);
    sum1 += clock();
    loadMat("Assets/TowerCheck/Curve2.txt", &data);

    sum1 -= clock();
    convolveOptimized(data);
    long long c3 = threshold(data.out, 135, ((long long) (data.w)) * data.h * data.ch);
    sum1 += clock();
    loadMat("Assets/TowerCheck/Curve3.txt", &data);

    sum1 -= clock();
    convolveOptimized(data);
    long long c4 = threshold(data.out, 135, ((long long) (data.w)) * data.h * data.ch);
    sum1 += clock();
    // saveFile(image, data, 0); //Not Optimzed ~~ Hori
    

    loadMat("Assets/TowerCheck/Vert.txt", &data);
    sum2 -= clock();
    convolveOptimized(data);
    long long c5 = threshold(data.out, 127, ((long long) (data.w)) * data.h * data.ch);
    sum2 += clock();

    loadMat("Assets/TowerCheck/Edge.txt", &data);
    sum2 -= clock();
    convolveOptimized(data);
    long long c6 = threshold(data.out, 50, ((long long) (data.w)) * data.h * data.ch);
    sum2 += clock();

    double score = calc(c1,c2,c3,c4,c5,c6, ((long long) (data.w)) * data.h * data.ch);

    long long sum = data.w * data.h;


    tidyup(&data);
    printf("*** %s ***\n", image);
    printf("\t %lf \t %lf \t %lf \t %lf", (double)c1/sum, (double)c2/sum, (double)c3/sum, (double)c4/sum);
    printf("\t %lf \t %lf\n", (double)c5/sum, (double)c6/sum);
    printf(isVicotrian ? "Victorian\n" : "Iraninan\n");
    printf(score < 0.4 ? "Likely Victorian": "Likely Iranian");
    printf("\n%lf", score);
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
