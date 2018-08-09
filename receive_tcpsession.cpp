/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：Receive_TcpSession.cpp
  * @author       著作权所有者: 张岩森
  * @brief        内容: 会话源程序，主要处理函数
  * @date         作成日期: 2018-07-27 14:49:34 周五
  * @date         修正日期：2018-07-27 14:49:34 周五
  ---------------------------------------------------------
  * */
#include "receive_tcpsession.h"
#include "mainwindow.h"

/**
  ---------------------------------------------------------
  * @file         File Name: Receive_TcpSession.cpp
  * @function     Function Name: Receive_TcpSession
  * @brief        Description: 构造函数，初始化网络连接信号
  * @date         Date: 2018-07-27 14:50:32 周五
  * @param        Parameter: parent 线程类型
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
Receive_TcpSession::Receive_TcpSession(Receive_TcpThread *parent)
{
    this->blockSize_ = 0;
    this->blockNumber_ = 0;
    this->thread_ = parent;
    receiveFile_ = new QFile();
    connect(this, &Receive_TcpSession::readyRead,
            this, &Receive_TcpSession::SlotStartRead);
    connect(this, &Receive_TcpSession::disconnected,
            this, &Receive_TcpSession::SlotDisConnected);
    connect(this, &Receive_TcpSession::SignalDoDisConnect,
            this, &Receive_TcpSession::SlotDoDisconnect);
    connect(this, &Receive_TcpSession::SignalDoConnectToServer,
            this, &Receive_TcpSession::SlotDoConnectToServer);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(SlotDisplayErrorMessage(QAbstractSocket::SocketError)));
}

Receive_TcpSession::~Receive_TcpSession()
{
    disconnect(this, &Receive_TcpSession::readyRead,
               this, &Receive_TcpSession::SlotStartRead);
    disconnect(this, &Receive_TcpSession::disconnected,
               this, &Receive_TcpSession::SlotDisConnected);
    disconnect(this, &Receive_TcpSession::SignalDoDisConnect,
               this, &Receive_TcpSession::SlotDoDisconnect);
    disconnect(this, &Receive_TcpSession::SignalDoConnectToServer,
               this, &Receive_TcpSession::SlotDoConnectToServer);
    disconnect(this, SIGNAL(error(QAbstractSocket::SocketError)),
               this, SLOT(SlotDisplayErrorMessage(QAbstractSocket::SocketError)));
}

// 断开连接
void Receive_TcpSession::Disconnect()
{
    qDebug() << "Receive_TcpSession::Disconnect threadID:"<< QThread::currentThreadId();
    emit this->SignalDoDisConnect();
}

void Receive_TcpSession::SlotDoDisconnect()
{
    this->disconnectFromHost();
}

void Receive_TcpSession::SlotDoConnectToServer(const QString &host, quint16 port)
{
    this->connectToHost(QHostAddress(host), port);
}

void Receive_TcpSession::ConnectToServer(const QString &host, quint16 port)
{
    emit this->SignalDoConnectToServer(host, port);
}

void Receive_TcpSession::SlotDisConnected()
{
    if(thread_)
        thread_->SubOne();
    // 通知会话断开连接
    if(OnDisConnected)
        OnDisConnected(this);
    emit this->SignalDisConnected(this);
}

void Receive_TcpSession::SlotStartRead()
{
    qDebug()<<"开始建立连接传输数据了";
    qDebug() << "Receive_TcpSession::SlotStartRead threadID:"<< QThread::currentThreadId();
    while(this->bytesAvailable() >= sizeof(quint64)) {
        if(blockSize_ == 0) {
            if(this->bytesAvailable() < sizeof(qint64)) {
                return ;
            }
            this->read((char *)&blockSize_, sizeof(qint64));
        }
        if(this->bytesAvailable() < blockSize_) {
            return ;
        }
        //        qDebug() << blockSize_ + sizeof(qint64);
        //        emit this->SignalRead(blockSize_ + sizeof(qint64));
        QByteArray data = this->read(blockSize_);
        processFileData(data);
        blockSize_ = 0;
    }
}

void Receive_TcpSession::processFileData(QByteArray &array)
{
    QDataStream in(&array, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_11);

    int key;
    QByteArray data;
    in >> key >> data;
    blockNumber_ ++;

    qDebug() << QString("已接收数据包:%1个").arg( blockNumber_);
    qDebug() << QString("收到标识符:%1 当前数据包大小:%2字节").arg(key).arg(data.size());

    extern unordered_map<unsigned int,User_Detail> user_map;
    QDir dir;
    QString userId = QString::number(user_map[this->peerAddress().toIPv4Address()].user_id);
    dir.cd(savePath);
    if(!dir.exists(userId)) {
        dir.mkdir(userId);
        qDebug() << "创建目录成功！";
    }
    switch(key) {
    case 0x01:
        receiveFileName_ = receiveFileName_.fromUtf8(data.data(), data.size());
        receiveFile_->setFileName(savePath + '/' + userId + '/' + receiveFileName_);
        emit this->SignalClientIP(this->peerAddress().toString());
        emit this->SignalReadFileName(receiveFileName_);
        emit this->SignalReadFilePath(receiveFile_->fileName());
        if(receiveFile_->exists()) {
            receiveFile_->remove();
        }
        if(!receiveFile_->open(QIODevice::WriteOnly)) {
            qDebug() << "不能进行文件写入！";
            break;
        }
        break;
    case 0x02: {
        QString size = QString::fromUtf8(data.data(), data.size());
        emit this->SignalReadFileSize(size.toUInt());
        break;
    }
    case 0x03:
        receiveFile_->write(data.data(), data.size());
        emit this->SignalRead(data.size());
        receiveFile_->flush();
        break;
    case 0x04:
        extern int number;
        ShowPicture::recognize(receiveFile_->fileName(), number);
        emit this->SignalReadFileFinish();
        receiveFile_->close();
        break;
    }
}

void Receive_TcpSession::SlotDisplayErrorMessage(QAbstractSocket::SocketError)
{
    qDebug() << QString("接收文件遇到错误:%1").arg(this->errorString());
    qDebug() << QString("正在移除文件:%1").arg(receiveFile_->fileName());
    if(receiveFile_->isOpen()) {
        receiveFile_->close();
    } else {
        return ;
    }
    if(!receiveFile_->fileName().isEmpty()) {
        receiveFile_->remove(receiveFileName_);
    } else {
        return ;
    }
}
