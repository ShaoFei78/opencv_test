#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;
// 任务一；霍夫圆检测
// 任务一的滑动条参数
int g_minDist   = 137;
int g_param1    = 17;
int g_param2    = 76;
int g_minRadius = 10;
int g_maxRadius = 460;


Mat g_img;
Mat g_blurred;
Mat g_result;

static const char* kWinName = "Hough Circle Result";

void onTrackbar(int, void*) {
    
    g_result = g_img.clone();

    vector<Vec3f> circles;
    HoughCircles(g_blurred, circles, HOUGH_GRADIENT,
                 1, g_minDist, g_param1, g_param2, g_minRadius, g_maxRadius);

    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(g_result, center, radius, Scalar(255, 0, 0), 3);
        circle(g_result, center, 3, Scalar(0, 0, 255), -1);
    }

    
    putText(g_result, "circles: " + to_string(circles.size()), Point(20, 40),
            FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 255, 0), 2);

    imshow(kWinName, g_result);
    
}

int main() {
    Mat img = imread("resource/test.png");
    // Check for failure
    if (img.empty()) {
        cout << "Could not read the image" << endl;
        return -1;
    }

    //预处理图片
    Mat gray;
    // 转灰度图
    cvtColor(img, gray, COLOR_BGR2GRAY);
    // 高斯模糊
    Mat blurred;
    GaussianBlur(gray, blurred, Size(9, 9), 2, 2);

    g_img = img;
    g_blurred = blurred;

    namedWindow(kWinName, WINDOW_AUTOSIZE);

    // 创建滑动条
    createTrackbar("Min Dist",   kWinName, &g_minDist,   200, onTrackbar);
    createTrackbar("Param1",     kWinName, &g_param1,    300, onTrackbar);
    createTrackbar("Param2",     kWinName, &g_param2,    100, onTrackbar);
    createTrackbar("Min Radius", kWinName, &g_minRadius, 300, onTrackbar);
    createTrackbar("Max Radius", kWinName, &g_maxRadius, 500, onTrackbar);

    onTrackbar(0, 0);  

    waitKey(0);        
    return 0;
}
