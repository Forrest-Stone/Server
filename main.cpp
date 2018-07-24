#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    VideoCapture capture(0);
    // 循环显示每一帧
    while(1){
        Mat frame;  // 定义一个Mat变量，用于存储每一帧的图像
        capture>>frame;  // 读取当前帧
        imshow("video",frame);  // 显示当前帧
        waitKey(30);  // 延时30ms
    }

    return a.exec();
}
