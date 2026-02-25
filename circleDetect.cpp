#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <string>
#include "utils.c"
#include <time.h>
#include "convolution.c"
#include "optimizedConvolution.c"
using namespace std;

typedef void (*ConvolutionFunc)(FilesDTO data);

// Global array of function pointers
ConvolutionFunc convFuncs[2] = { convolve, convolveOptimized };

clock_t detect(char* imagePath, char* matrixPath, int isOpt) {

    cv::Mat original = cv::imread(imagePath, cv::IMREAD_COLOR);
    
    FilesDTO data = {0};

    clock_t time = 0;

    ConvolutionFunc conv = convFuncs[isOpt ? 1 : 0];

    // لود تصویر و ماتریس
    prepare(imagePath, matrixPath, &data, 1);
    time -= clock();
    conv(data);
    memcpy(data.in, data.out, data.ch*data.w*data.h);
    time += clock();
    loadMat("Assets/RectCheck/Edge.txt", &data);
    time -= clock();
    conv(data);
    time += clock();

    // saveFile(imagePath, data, 1);
    cv::Mat gray(data.h, data.w, CV_8UC1, data.out);

    // پیدا کردن کانتور‌ها
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // رسم دایره‌ها روی تصویر رنگی اصلی
    for (size_t i = 0; i < contours.size(); i++) {

        // حذف کانتورهای کوچک
        double area = cv::contourArea(contours[i]);
        if (area < 100)
            continue;

        // محاسبه حداقل دایره محاطی
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[i], center, radius);

        // بررسی دایره بودن تقریبی
        double circleArea = CV_PI * radius * radius;
        double ratio = area / circleArea;

        if (ratio > 0.7 && ratio < 1.3) {  
            // رسم دایره روی تصویر رنگی اصلی
            cv::circle(original, center, (int)radius, cv::Scalar(0,0,255), 2);
        }
    }

    char end[50];
    sprintf(end, "Convolved/%s", imagePath);
    cv::imwrite(end, original);

    printf("%2.6fms\n", (double)(time) / CLOCKS_PER_SEC * 1000);
    return time;
}

int main() {
    char path[50];
    char mat[50] = "Assets/RectCheck/Blur.txt";
    clock_t time = 0, optTime = 0;
    printf("Non optimized: \n");
    for (int i = 1; i <= 16; i++) {
        sprintf(path, "Assets/RectCheck/%d.jpg", i);
        time += detect(path, mat, 0);
    }
    printf("\n\n\nOptimized: \n");
    for (int i = 1; i <= 16; i++) {
        sprintf(path, "Assets/RectCheck/%d.jpg", i);
        optTime += detect(path, mat, 1);
    }

    printf("Optimized Version is faster by: \t %2.3f%%\n", (double) (time) / (optTime) * 100 - 100);
}