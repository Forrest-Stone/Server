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
#include <QTableWidgetItem>

#include "receive_tcpserver.h"
#include "server_login_dialog.h"
#include "login_handler.h"
#include "sessioninfo.h"
#include "sessioninfolist.h"
#include "showpicture.h"

#define STARTSERVER "开启监听"
#define STOPSERVER "关闭监听"
#define PORTNUM 8888

extern QString savePath;

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
    void AcceptSession(std::shared_ptr<Receive_TcpSession> &tcpSession);

<<<<<<< HEAD
public slots:
    void SlotChangeRecState(int row_num);
=======
    QString GetFileSavePath();
    void sendFileSavePath();
>>>>>>> 6482fa827a8dc27e940237bfe3379b0cf2d78f56
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void SlotDisConnected();
    void SlotRead(qint64 size);
    void SlotReadClient(QString client);

    void SlotReadConnect(QString info);
<<<<<<< HEAD
    void on_treeWidget_doubleClicked(const QModelIndex &index);
=======
    void SlotReadFileName( QString fileName);
    void SlotReadFilePath(QString path);
    void SlotReadFileSize(qint64 size);
>>>>>>> 6482fa827a8dc27e940237bfe3379b0cf2d78f56

private:
    Ui::MainWindow *ui;
    Receive_TcpServer *server_ = nullptr;
    Server_Login_Dialog *login  = nullptr;
    ShowPicture *sp = nullptr;
    SessionInfoList sessionList_;
    void Write(const QString &msg);
    QString GetHostIpAddr();
};

#endif // MAINWINDOW_H
