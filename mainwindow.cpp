﻿/**
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

    // 初始化整体布局
    QFile file(":/style/default.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);

    ui->lineEdit->setPlaceholderText(GetHostIpAddr());
    ui->lineEdit_2->setPlaceholderText(QString::number(PORTNUM));
    ui->lineEdit_3->setPlaceholderText(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    login= new Server_Login_Dialog(this);
    server_ = new Receive_TcpServer(this);
    sp = new ShowPicture(this);
    cm = new ChargeManage(this);
    charge_ = new ChargeDialog(this);
    car_ = new Count_Dialog(this);
    server_->OnAccepted = std::bind(&MainWindow::AcceptSession, this, std::placeholders::_1);
    connect(server_, &Receive_TcpServer::SignalReadConnect,
            this, &MainWindow::SlotReadConnect);
    connect(sp,SIGNAL(has_check(int)),this,SLOT(SlotCheckOut(int)));
    connect(sp,SIGNAL(car_in(QString,int,QDateTime)),cm,SLOT(carIn(QString,int,QDateTime)));
    connect(sp,SIGNAL(car_out(QString,int,QDateTime)),cm,SLOT(carOut(QString,int,QDateTime)));
    connect(ui->actionjifei, SIGNAL(triggered(bool)),
            this, SLOT(SlotCharge()));
    connect(ui->actionliuliang, SIGNAL(triggered(bool)),
            this, SLOT(SlotCar()));
    login->show();
    this->sendFileSavePath();
}

MainWindow::~MainWindow()
{
    if(server_) {
        server_->Stop();
        delete server_;
    }
    server_ = nullptr;
    delete ui;
}


/**
  ---------------------------------------------------------
  * @file         File Name: mainwindows.cpp
  * @function     Function Name: on_pushButton_clicjed()
  * @brief        Description: 开启服务器监听
  * @date         Date: 2018-08-02 09:40:30 周四
  * @param        Parameter: Tags
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void MainWindow::on_pushButton_clicked()
{
    if(STARTSERVER == this->ui->pushButton->text()) {
        uint16_t port = ui->lineEdit_2->text().toInt();
        ServerData serverData;
        serverData.portNum = port;
        if(server_->Start(serverData)) {
            QString startMessage = QString("接收文件监听开启！");
            startMessage += QString(" 服务器IP：" + GetHostIpAddr());
            startMessage += QString(" 监听端口：" + QString::number(serverData.portNum));
            startMessage += QString(" " + GetCurrentTime());
            this->Write(startMessage);
        } else {
            QString errorMessage = QString("启动文件监听服务失败！");
            errorMessage += QString(" " + GetCurrentTime());
            this->Write(errorMessage);
        }
        this->ui->pushButton->setText(STOPSERVER);
    } else if(STOPSERVER == this->ui->pushButton->text()) {
        this->ui->pushButton->setText(STARTSERVER);
        QString closeMessage = QString("接收文件监听关闭！");
        closeMessage += QString(" " + GetCurrentTime());
        this->Write(closeMessage);
        server_->Stop();
    }
}

void MainWindow::AcceptSession(std::shared_ptr<Receive_TcpSession> &tcpSession)
{
    qDebug() << "主窗口关联接收的请求！";
    SessionInfo *info = this->sessionList_.NewSessionInfo(tcpSession);
    connect(info, &SessionInfo::SignalDisconnect,
            this, &MainWindow::SlotDisConnected);
    connect(info, &SessionInfo::SignalRead,
            this, &MainWindow::SlotRead);
    connect(info, &SessionInfo::SignalReadClient,
            this, &MainWindow::SlotReadClient);
    connect(info, &SessionInfo::SignalReadFileName,
            this, &MainWindow::SlotReadFileName);
    connect(info, &SessionInfo::SignalReadFilePath,
            this, &MainWindow::SlotReadFilePath);
    connect(info, &SessionInfo::SignalReadFileSize,
            this, &MainWindow::SlotReadFileSize);
    connect(info, &SessionInfo::SignalReadFinish,
            this, &MainWindow::SlotReadFinish);
    //    connect(info, &SessionInfo::SignalProgressBar,
    //            this, &MainWindow::SlotRead);
}

void MainWindow::Write(const QString &msg)
{
    this->ui->plainTextEdit->appendPlainText(msg);
}

/**
  ---------------------------------------------------------
  * @file         File Name: mainwindows.cpp
  * @function     Function Name: GetHostIpAddr
  * @brief        Description: 获取服务器 IP 地址
  * @date         Date: 2018-08-01 21:05:57 周三
  * @param        Parameter: Tags
  * @return       Return Code: QString
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
QString MainWindow::GetHostIpAddr()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果没有找到，则以本地IP地址为IP
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return strIpAddress;

}

/**
  ---------------------------------------------------------
  * @file         File Name: mainwindow.cpp
  * @function     Function Name: on_pushButton_2_clicked()
  * @brief        Description: 选择文件保存路径
  * @date         Date: 2018-08-02 09:55:44 周四
  * @param        Parameter: Tags
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void MainWindow::on_pushButton_2_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, "选择文件保存路径",
                                                         QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    this->ui->lineEdit_3->setText(filePath);
    this->sendFileSavePath();
}

// 获取当前系统时间 输出结果到界面显示
QString MainWindow::GetCurrentTime()
{
    QString currentTime = QString(" 时间：");
    currentTime += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    return currentTime;
}

// 获取保存路径
QString MainWindow::GetFileSavePath()
{
    QString path;
    if(NULL == ui->lineEdit_3->text()) {
        path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    } else {
        path = ui->lineEdit_3->text();
    }
    return path;
}

void MainWindow::sendFileSavePath()
{
    savePath = this->GetFileSavePath();
}

void MainWindow::SlotCheckOut(int row_num)
{
    ui->tableWidget_2->setItem(row_num, 6, new QTableWidgetItem("是"));
}

void MainWindow::SlotDisConnected()
{
    this->Write("Disconnected");
}

//void MainWindow::SlotRead(qint64 size)
//{
//    //    QString
//    //    QString msg = data;
//    //    this->Write(msg);
//    //    info->Write(data.toStdString().c_str(), size);
//}

void MainWindow::SlotReadConnect(QString info)
{
    QString clietInfo = QString("服务器接收客户端：" + info);
    clietInfo += QString(" 的发送文件请求！");
    clietInfo += GetCurrentTime();
    this->Write(clietInfo);
}

/**
  ---------------------------------------------------------
  * @file         File Name: mainwindows.cpp
  * @function     Function Name: SlotReadClient
  * @brief        Description: 在主界面显示客户端连接状态
  * @date         Date: 2018-08-06 14:26:54 周一
  * @param        Parameter: Sesssion *info QString client
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void MainWindow::SlotReadClient(QString client)
{
    extern int number;
    progressBar_ = new QProgressBar;
    progressBar_->setRange(0, 100);
    progressBar_->setValue(0);
    int rowNum = ui->tableWidget_2->rowCount();
    number = rowNum;
    ui->tableWidget_2->insertRow(rowNum);
    ui->tableWidget_2->setItem(rowNum, 0, new QTableWidgetItem(client));
    ui->tableWidget_2->setCellWidget(rowNum, 3, progressBar_);
    ui->tableWidget_2->setItem(rowNum, 5, new QTableWidgetItem("否"));
    ui->tableWidget_2->setItem(rowNum, 6, new QTableWidgetItem("否"));
    QString clietInfo = QString("客户端IP：" + client);
    clietInfo += QString(" 向服务器发送文件 ");
    clietInfo += GetCurrentTime();
    this->Write(clietInfo);
}

void MainWindow::SlotReadFileName(QString fileName)
{
    int rowNum = ui->tableWidget_2->rowCount() - 1;
    ui->tableWidget_2->setItem(rowNum, 1, new QTableWidgetItem(fileName));
}

void MainWindow::SlotReadFilePath(QString path)
{
    int rowNum = ui->tableWidget_2->rowCount() - 1;
    ui->tableWidget_2->setItem(rowNum, 4, new QTableWidgetItem(path));
}

void MainWindow::SlotReadFileSize(qint64 size)
{
    int rowNum = ui->tableWidget_2->rowCount() - 1;
    ui->tableWidget_2->setItem(rowNum, 2, new QTableWidgetItem(QString::number(size)));
    receiveSize_ = 0;
    totalSize_ = size;
    progressBar_->setRange(0, size);
    progressBar_->setValue(0);
}


void MainWindow::SlotRead(qint64 size)
{
    receiveSize_ += size;
    progressBar_->setValue(receiveSize_);
    qDebug() << QString("当前数据包大小:%1字节 已接收字节:%2 总共字节:%3")
                .arg(size).arg(receiveSize_).arg(totalSize_);
    qApp->processEvents();
}

//void MainWindow::SlotUpdatePBar(qint64 size)
//{
//    progressBar_->setValue(size);
//}

void MainWindow::SlotReadFinish()
{

    int rowNum = ui->tableWidget_2->rowCount() - 1;
    //progressBar_->setValue(100);
    ui->tableWidget_2->setItem(rowNum, 5, new QTableWidgetItem("是"));
}

void MainWindow::on_tableWidget_2_doubleClicked(const QModelIndex &index)
{
    sp->myshow(index.row());
}

void MainWindow::SlotCharge()
{
    charge_->show();
}

void MainWindow::SlotCar()
{
    car_->show();
}
