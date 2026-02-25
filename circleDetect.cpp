#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <string>
#include "utils.c"
using namespace std;

int detect(char* imagePath, char* matrixPath) {

    // خواندن تصویر اصلی (رنگی)
    cv::Mat original = cv::imread(imagePath, cv::IMREAD_COLOR);
    
    FilesDTO data = {0};

    // لود تصویر و ماتریس
    prepare(imagePath, matrixPath, &data, 1);

    // تبدیل بافر ورودی به cv::Mat
    cv::Mat gray(data.h, data.w, CV_8UC1, data.in);

    cv::Mat blurred, edges;

    // کاهش نویز
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 1.5);

    // استخراج لبه
    cv::Canny(blurred, edges, 50, 150);

    // پیدا کردن کانتور‌ها
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

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

    printf("دایره‌ها رسم شدند روی تصویر اصلی.\n");
    return 0;
}

int main() {
    char path[50];
    char mat[50] = "Assets/RectCheck/Blur.txt";
    for (int i = 1; i <= 17; i++) {
        sprintf(path, "Assets/RectCheck/%d.jpg", i);
        detect(path, mat);
    }
}