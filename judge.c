#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "optimizedConvolution.c"
#include "utils.c"
#include "judgeThreshold.c"

void checkImage(char *image) {
    FilesDTO data = {0};
    prepare(image, "Assets/TowerCheck/Vert.txt", &data, 1);

    clock_t start1 = clock();
    convolveOptimized(data);
    long long c1 = threshold(data.out, 127, ((long long) (data.w)) * data.h * data.ch);
    clock_t end1 = clock();
    saveFile(image, data, 0); //Not Optimzed ~~ Vert

    loadMat("Assets/TowerCheck/Hori.txt", &data);
    clock_t start2 = clock();
    convolveOptimized(data);
    long long c2 = threshold(data.out, 127, ((long long) (data.w)) * data.h * data.ch);
    clock_t end2 = clock();
    saveFile(image, data, 1); //Optimzed ~~ Hori

    tidyup(&data);
    printf("*** %s ***\n", image);
    printf("Vertical check took: \t\t %2.6fms\t\n", (double)(end1 - start1) / CLOCKS_PER_SEC * 1000);
    printf("Horizental check took: \t\t %2.6fms\n", (double)(end2 - start2) / CLOCKS_PER_SEC * 1000);
    printf("Vertical sharp edges count: \t %lld\n", c1);
    printf("Horizental sharp edges count: \t %lld\n", c2);
    printf("%f\n", ((double) c1) / c2);
    printf(c1 > c2 ? "It's a tower." : "It's an Iranian historical architecture!");
    printf("\n****************************************\n\n\n");
}

int main() {
    char path[50];
    for (int i = 1; i < 12; i++) {
        sprintf(path, "Assets/TowerCheck/%d.jpg", i);
        checkImage(path);
    }

    return 0;
}
