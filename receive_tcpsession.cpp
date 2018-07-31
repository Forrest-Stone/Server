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
    //    connect(this, &Receive_TcpSession::readyRead,
    //            this, &Receive_TcpSession::SlotReadFile);
    connect(this, &Receive_TcpSession::readyRead,
            this, &Receive_TcpSession::SlotStartRead);
    connect(this, &Receive_TcpSession::disconnected,
            this, &Receive_TcpSession::SlotDisConnected);
    connect(this, &Receive_TcpSession::SignalDoDisConnect,
            this, &Receive_TcpSession::SlotDoDisconnect);
    connect(this, &Receive_TcpSession::SignalDoWrite,
            this, &Receive_TcpSession::SlotDoWrite);
    connect(this, &Receive_TcpSession::SignalDoConnectToServer,
            this, &Receive_TcpSession::SlotDoConnectToServer);
    connect(this, &Receive_TcpSession::SignalMessage,
            this, &Receive_TcpSession::SignalMessage);
}

/**
  ---------------------------------------------------------
  * @file         File Name: Receive_TcpSession.cpp
  * @function     Function Name: ~Receive_TcpSession
  * @brief        Description: 析构函数，处理断开连接的会话
  * @date         Date: 2018-07-27 14:52:46 周五
  * @param        Parameter: Tags
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
Receive_TcpSession::~Receive_TcpSession()
{
    disconnect(this, &Receive_TcpSession::readyRead,
               this, &Receive_TcpSession::SlotStartRead);
    disconnect(this, &Receive_TcpSession::disconnected,
               this, &Receive_TcpSession::SlotDisConnected);
    disconnect(this, &Receive_TcpSession::SignalDoDisConnect,
               this, &Receive_TcpSession::SlotDoDisconnect);
    disconnect(this, &Receive_TcpSession::SignalDoWrite,
               this, &Receive_TcpSession::SlotDoWrite);
    disconnect(this, &Receive_TcpSession::SignalDoConnectToServer,
               this, &Receive_TcpSession::SlotDoConnectToServer);
    disconnect(this, &Receive_TcpSession::SignalMessage,
               this, &Receive_TcpSession::SignalMessage);
}

void Receive_TcpSession::ConnectToServer(const QString &host, quint16 port)
{
    emit this->SignalDoConnectToServer(host, port);
}

// 断开连接
void Receive_TcpSession::Disconnect()
{
    emit this->SignalDoDisConnect();
}

void Receive_TcpSession::Write(const char *data, qint64 len)
{
    writeBuffer_ = QByteArray(data, len);
    emit this->SignalDoWrite();
}

void Receive_TcpSession::SlotDoConnectToServer(const QString &host, quint16 port)
{
    this->connectToHost(QHostAddress(host), port);
}

void Receive_TcpSession::SlotStartRead()
{
    //    while(socket_->bytesAvailable() >= sizeof(quint64)) {
    //        if(blockSize_ == 0) {
    //            if(socket_->bytesAvailable() < sizeof(qint64)) {
    //                return ;
    //            }
    //            socket_->read((char *)&blockSize_, sizeof(qint64));
    //        }
    //        if(socket_->bytesAvailable() < blockSize_) {
    //            return;
    //        }
    //        emit this->SignalRead(blockSize + sizeof(qint64));
    //        QByteArray data = s->read(blockSize);
    //        proccessData(data);
    //        blockSize = 0;
    //    }
    buffer_ = this->readAll();
    if(OnRead)
        OnRead(buffer_);
    processFileData(buffer_);
    emit this->SignalRead(buffer_.toStdString().c_str(), buffer_.length());
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

void Receive_TcpSession::SlotDoWrite()
{
    this->write(writeBuffer_);
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

    emit this->SignalMessage(QString("已接收数据包:%1个").arg( blockNumber_));
    emit this->SignalMessage(QString("收到标识符:'%1' 当前数据包大小:'%2'字节").arg(key).arg(data.size()));

    switch(key) {
    case 0x01:
        receiveFileName_ = receiveFileName_.fromUtf8(data.data(), data.size());
        receiveFile_->setFileName(receiveFileName_);
//        receiveFile_.setFileName(qApp->applicationDirPath() + "/" + fileName);
        emit this->SignalReadFileName(receiveFile_->fileName());
        if(receiveFile_->exists()) {
            receiveFile_->remove();
        }
        if(!receiveFile_->open(QIODevice::WriteOnly)) {
            emit this->SignalMessage("不能打开文件进行写入");
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
        socket_->disconnectFromHost();
        break;
    }
}
