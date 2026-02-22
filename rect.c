#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "optimizedConvolution.c"
#include "utils.c"

const unsigned char THRESHOLD = 127;


void checkImage(char *image) {
    FilesDTO vert = {0}, hori = {0}, edge = {0}, input = {0};
    prepare(image, "Assets/RectCheck/Blur.txt", &vert, 1);
    prepare(image, "Assets/RectCheck/Blur.txt", &input, 0);
    convolveOptimized(vert);

    long long size = ((long long) (vert.w)) * vert.h * vert.ch;
    memcpy(vert.in, vert.out, size);

    prepare(NULL, "Assets/RectCheck/Hori.txt", &hori, 1);
    prepare(NULL, "Assets/RectCheck/Edge.txt", &edge, 1);
    copyDTO(&hori, &vert);
    copyDTO(&edge, &vert);

    printf("%lld", size);
    clock_t sum = 0;
    sum -= clock();
    convolveOptimized(vert);
    convolveOptimized(hori);
    convolveOptimized(edge);
    for (int i = 0; i < size; i++) {
        if (i%(size/10) == 0) printf("%d\n", i);
        if (edge.out[i] <= THRESHOLD) continue;
        for (int j = size; j > i + vert.w * 3; j--) {
            int ix = i / vert.w, iy = i % vert.w, jx = j / vert.w, jy = j % vert.w;
            if (jx > jy - 4) continue;
            int isRect = 1;

            for (int x = ix; x <= jx; x++) {
                if (hori.out[x * vert.w + iy] <= THRESHOLD || 
                    hori.out[x * vert.w + jy] <= THRESHOLD) {
                    isRect = 0;
                    break;
                }
            }
            if (!isRect) continue;

            // 2. Validate left and right vertical edges
            for (int y = iy; y <= jy; y++) {
                if (vert.out[ix * vert.w + y] <= THRESHOLD || 
                    vert.out[jx * vert.w + y] <= THRESHOLD) {
                    isRect = 0;
                    break;
                }
            }

            // 3. If it's a rectangle, trace it in RED on the input image
            if (isRect) {
                for (int x = ix; x <= jx; x++) {
                    int p1 = (x * vert.w + iy) * vert.ch;
                    int p2 = (x * vert.w + jy) * vert.ch;
                    input.in[p1] = 255; input.in[p1+1] = 0; input.in[p1+2] = 0; // Red
                    input.in[p2] = 255; input.in[p2+1] = 0; input.in[p2+2] = 0;
                }
                for (int y = iy; y <= jy; y++) {
                    int p1 = (ix * vert.w + y) * vert.ch;
                    int p2 = (jx * vert.w + y) * vert.ch;
                    input.in[p1] = 255; input.in[p1+1] = 0; input.in[p1+2] = 0;
                    input.in[p2] = 255; input.in[p2+1] = 0; input.in[p2+2] = 0;
                }
                // Break if you only want to find the largest/first rectangle per seed point
                break; 
            }
        }
    }

    sum += clock();
    input.out = input.in;
    saveFile(image, input, 0);

}

int main() {
    char path[50];
    for (int i = 1; i < 2; i++) {
        sprintf(path, "Assets/RectCheck/%d.jpg", i);
        checkImage(path);
    }

    return 0;
}
