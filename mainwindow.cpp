/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：mainwindow.cpp
  * @author       著作权所有者: 张岩森
  * @brief        内容: 主窗口源程序
  * @date         作成日期: 2018-07-27 14:49:02 周五
  * @date         修正日期：2018-07-27 14:49:02 周五
  ---------------------------------------------------------
  * */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server_ = new TcpServer();
    //    Server_->OnAccepted = std::bind(&MainWindow::AcceptSession, this, std::placeholders::_1);
    //    server_->OnAccepted = std::bind()
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::AcceptSession(std::shared_ptr<NetAPI::TcpSession> &tcpSession)
//{
//    SessionInfo *info = this->SessionList_.NewSessionInfo(tcpSession);
//    connect(info, &SessionInfo::SignalDisconnect, this, &MainWindow::SlotDisConnected);
//    connect(info, &SessionInfo::SignalRead, this, &MainWindow::SlotRead);
//    this->WriteLog("Accept One");
//}

void MainWindow::on_pushButton_clicked()
{
    uint16_t port = ui->lineEdit_2->text().toInt();
    ServerData serverData;
    serverData.portNum = port;
    if(server_->Start(serverData)) {
//        qDebug() << serverData.portNum << " " << serverData.threadNum;
        this->Write("Start Successful!");
    } else {
        this->Write("Could Not Start Server!");
    }
}

void MainWindow::Write(const QString &msg)
{
    this->ui->plainTextEdit->appendPlainText(msg);
}

