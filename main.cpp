/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：main.cpp
  * @author       著作权所有者: 张岩森
  * @brief        内容: 主函数
  * @date         作成日期: 2018-07-27 14:48:34 周五
  * @date         修正日期：2018-07-27 14:48:34 周五
  ---------------------------------------------------------
  * */
#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QTextCodec>
#include <unordered_map>
#include "receive_tcpserver.h"
#include "user_detail.h"
#include "showpicture.h"
#include "connection.h"

#include "chargemanage.h"
#include "chargedialog.h"
#include <QDateTime>

using namespace std;
using namespace cv;

unordered_map<unsigned int,User_Detail> user_map;
QString savePath;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    createConnection();
    createTables();


    return a.exec();
}
