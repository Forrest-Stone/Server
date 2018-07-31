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
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    login= new Server_Login_Dialog(this);
    server_ = new Receive_TcpServer();
    login->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    uint16_t port = ui->lineEdit_2->text().toInt();
    ServerData serverData;
    serverData.portNum = port;
    if(server_->Start(serverData)) {
        this->Write("Start Successful!");
    } else {
        this->Write("Could Not Start Server!");
    }
}

void MainWindow::Write(const QString &msg)
{
    this->ui->plainTextEdit->appendPlainText(msg);
}

