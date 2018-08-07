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
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>

#include "receive_tcpserver.h"
#include "server_login_dialog.h"
#include "login_handler.h"
#include "sessioninfo.h"
#include "sessioninfolist.h"

#define STARTSERVER "开启监听"
#define STOPSERVER "关闭监听"
#define PORTNUM 8888


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    friend class Server_Login_Dialog;
    friend class login_handler;

    QString GetCurrentTime();
    QString GetFileSavePath();
    void AcceptSession(std::shared_ptr<Receive_TcpSession> &tcpSession);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void SlotDisConnected();
    void SlotRead(SessionInfo *info, qint64 size);
    void SlotReadClient(SessionInfo *info, QString);

    void SlotReadConnect(QString info);
private:
    Ui::MainWindow *ui;
    Receive_TcpServer *server_ = nullptr;
    Server_Login_Dialog *login  = nullptr;
    SessionInfoList sessionList_;
    void Write(const QString &msg);
    QString GetHostIpAddr();
};

#endif // MAINWINDOW_H
