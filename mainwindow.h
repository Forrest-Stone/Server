/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：mainwindow.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 主窗口头文件
  * @date         作成日期: 2018-07-27 14:45:48 周五
  * @date         修正日期：2018-07-27 14:45:48 周五
  ---------------------------------------------------------
  * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "receive_tcpserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Receive_TcpServer *server_ = nullptr;
    void Write(const QString &msg);
};

#endif // MAINWINDOW_H
