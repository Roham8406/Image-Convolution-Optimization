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

ConvolutionFunc convFuncs[2] = { convolve, convolveOptimized }; //فانکشن پوینتر برای محاسبه مدت زمان بهینگی

clock_t detect(char* imagePath, int isOpt) {

    cv::Mat original = cv::imread(imagePath, cv::IMREAD_COLOR); //عکس سه کاناله برای رسم نشانه‌ها
    
    FilesDTO data = {0};
    clock_t time = 0;
    ConvolutionFunc conv = convFuncs[isOpt ? 1 : 0];

    prepare(imagePath, "Assets/RectCheck/Blur.txt", &data, 1);  //عکس یک کاناله برای آنالیز
    time -= clock();
    conv(data);                                                 //کانولوشن بلور گاوسی
    memcpy(data.in, data.out, data.ch*data.w*data.h);           //آماده سازی کانولوشون دوم
    time += clock();
    loadMat("Assets/RectCheck/Edge.txt", &data);
    time -= clock();
    conv(data);                                                 //کانولوشن پیدا کردن لبه‌ها

    // saveFile(imagePath, data, 1);
    cv::Mat gray(data.h, data.w, CV_8UC1, data.out);            //تبدیل داده خام به cv

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, 
        cv::CHAIN_APPROX_SIMPLE);                               //بدست آوردن کانتورهای عکس کانوالو شده

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area < 100) continue;                               //صرف نظر از کانتورهای کوچک

        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[i], center, radius);    //محاسبه دایره‌ٔ محاطی مینیمال

        double circleArea = CV_PI * radius * radius;            //بررسی میزان خمیدگی دایره
        double ratio = area / circleArea;

        if (ratio > 0.7 && ratio < 1.3) {  
            cv::circle(original, center, (int)radius,
                        cv::Scalar(0,0,255), 2);                //رسم دایره
        }
    }
    time += clock();

    char end[50];
    sprintf(end, "Convolved/%s%s", isOpt ? "Opt_" : "",imagePath);
    cv::imwrite(end, original);                                 //ذخیره تصویر

    printf("%2.6fms\n", (double)(time) / CLOCKS_PER_SEC*1000);  //چاپ زمان مفید آنالیز
    return time;
}

int main() {
    char path[50];
    clock_t time = 0, optTime = 0;
    // printf("Non optimized: \n");
    for (int i = 50; i <= 100; i++) {
        printf("%d\n", i);
        sprintf(path, "Assets/RectCheck/%d.jpg", i);
        time += detect(path, 0);                                //بررسی تصاویر با کانولوشن غیر بهینه
    // }
    // printf("\n\n\nOptimized: \n");
    // for (int i = 7; i <= 33; i++) {
    //     sprintf(path, "Assets/RectCheck/%d.jpg", i);
        optTime += detect(path, 1);     
        printf("\n");                        //بررسی تصاویر با کانولوشن بهینه
    }

    printf("Optimized Version is faster by: \t %2.3f%%\n",
            (double) (time) / (optTime) * 100 - 100);           //محاسبهٔ میزان بهینگی
}