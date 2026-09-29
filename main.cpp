#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;
//任务二：颜色提取
Mat imgHSV;              
Mat g_img;               
Mat g_result;
Mat y_mask, b_mask;
//滑动条参数
int y_hmin=23,y_smin=72,y_vmin=90;
int y_hmax=37,y_smax=255,y_vmax=255;
int b_hmin=77,b_smin=90,b_vmin=33;
int b_hmax=124,b_smax=255,b_vmax=255;
static const string windowName = "Color Detection";
//回调函数
void onTrackbar(int,void*){
    Scalar y_lower(y_hmin,y_smin,y_vmin);
    Scalar y_upper(y_hmax,y_smax,y_vmax);
    Scalar b_lower(b_hmin,b_smin,b_vmin);
    Scalar b_upper(b_hmax,b_smax,b_vmax);

    inRange(imgHSV,y_lower,y_upper,y_mask);
    inRange(imgHSV,b_lower,b_upper,b_mask);

    Mat mask;
    bitwise_or(y_mask, b_mask, mask);   // 黄色和蓝色合并成一张掩膜

    //g_result = Mat::zeros(g_img.size(), g_img.type());
    //g_img.copyTo(g_result, mask);       // 只保留检测到的颜色，其余变黑
    g_result = mask;

    imshow(windowName,g_result);
    //框颜色
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(mask,mask,MORPH_CLOSE,kernel);
    morphologyEx(mask,mask,MORPH_OPEN,kernel);
    vector<vector<Point>> contours;
    findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    Mat draw_img=g_img.clone();
    for(const auto& contour:contours){
        if(contourArea(contour)<200) continue;
        Rect rect=boundingRect(contour);
        rectangle(draw_img,rect,Scalar(0,0,255),2);
    }
    imshow("Detected Colors", draw_img);
}
int main() {
    // 读取图像
    Mat image = imread("resource/task2.png");
    if (image.empty()) {
        cout << "无法读取图像" << endl;
        return -1;
    }

    // 将图像转换为灰度图
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);

    // 显示原始图像和灰度图
    imshow("Image", image);
    imshow("GrayImage", grayImage);

    cvtColor(image,imgHSV,COLOR_BGR2HSV);
    g_img = image;                       // 显示用原图，不是 HSV 图

    namedWindow(windowName);
    createTrackbar("Y_Hmin", windowName, &y_hmin, 180, onTrackbar);
    createTrackbar("Y_Smin", windowName, &y_smin, 255, onTrackbar);
    createTrackbar("Y_Vmin", windowName, &y_vmin, 255, onTrackbar);
    createTrackbar("Y_Hmax", windowName, &y_hmax, 180, onTrackbar);
    createTrackbar("Y_Smax", windowName, &y_smax, 255, onTrackbar);
    createTrackbar("Y_Vmax", windowName, &y_vmax, 255, onTrackbar);
    createTrackbar("B_Hmin", windowName, &b_hmin, 180, onTrackbar);
    createTrackbar("B_Smin", windowName, &b_smin, 255, onTrackbar);
    createTrackbar("B_Vmin", windowName, &b_vmin, 255, onTrackbar);
    createTrackbar("B_Hmax", windowName, &b_hmax, 180, onTrackbar);
    createTrackbar("B_Smax", windowName, &b_smax, 255, onTrackbar);
    createTrackbar("B_Vmax", windowName, &b_vmax, 255, onTrackbar);
    onTrackbar(0,0);
    waitKey(0);
    return 0;
}
