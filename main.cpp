﻿#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QTextCodec>

#pragma execution_character_set("utf-8")

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    // 查看数据库中可用的驱动
    qDebug() << "可用的驱动：";
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        qDebug() << driver;
    }

    // 创建连接，使用本地的 MySQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mysql");
    db.setPort(3306);
    db.setUserName("zys");
    db.setPassword("zyszuibang");
    bool ok = db.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else {
        qDebug() << "数据库连接失败";

    }

    VideoCapture capture(0);
    // 循环显示每一帧
    while(1) {
        Mat frame;  // 定义一个Mat变量，用于存储每一帧的图像
        capture>>frame;  // 读取当前帧
        imshow("video",frame);  // 显示当前帧
        waitKey(30);  // 延时30ms
    }

    return a.exec();
}
