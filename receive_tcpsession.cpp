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
}

Receive_TcpSession::~Receive_TcpSession()
{
    disconnect(this, &Receive_TcpSession::readyRead,
               this, &Receive_TcpSession::SlotStartRead);
    disconnect(this, &Receive_TcpSession::disconnected,
               this, &Receive_TcpSession::SlotDisConnected);
    disconnect(this, &Receive_TcpSession::SignalDoDisConnect,
               this, &Receive_TcpSession::SlotDoDisconnect);
}


// 断开连接
void Receive_TcpSession::Disconnect()
{
    qDebug() << "Receive_TcpSession::Disconnect threadID:"<< QThread::currentThreadId();
    emit this->SignalDoDisConnect();
}


void Receive_TcpSession::SlotStartRead()
{
    qDebug()<<"开始建立连接传输数据了";
    qDebug() << "Receive_TcpSession::SlotStartRead threadID:"<< QThread::currentThreadId();
    while(this->bytesAvailable() >= sizeof(quint64)) {
        qDebug() << this->bytesAvailable();
        if(blockSize_ == 0) {
            if(this->bytesAvailable() < sizeof(qint64)) {
                return ;
            }
            this->read((char *)&blockSize_, sizeof(qint64));
        }
        if(this->bytesAvailable() < blockSize_) {
            return ;
        }
        emit SignalRead(blockSize_ + sizeof(qint64));
        QByteArray data = this->read(blockSize_);
        processFileData(data);
        blockSize_ = 0;
        qDebug() << this->bytesAvailable();
    }
}

void Receive_TcpSession::SlotDisConnected()
{
    if(thread_)
        thread_->SubOne();
    //通知会话断开连接
    if(OnDisConnected)
        OnDisConnected(this);
    emit this->SignalDisConnected(this);
}

void Receive_TcpSession::SlotDoDisconnect()
{
    this->disconnectFromHost();
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

    switch(key) {
    case 0x01:
        receiveFileName_ = receiveFileName_.fromUtf8(data);
        qDebug() << receiveFileName_;
        receiveFile_->setFileName(QString(QCoreApplication::applicationDirPath()) + '/' + receiveFileName_);
        emit this->SignalReadFileName(receiveFile_->fileName());
        emit this->SignalClientIP(this->peerAddress().toString());
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
        receiveFile_->flush();
        break;
    case 0x04:
        receiveFile_->close();
        break;
    }
}
