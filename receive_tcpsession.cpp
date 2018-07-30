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
    this->thread_ = parent;
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
    buffer_ = this->readAll();
    if(OnRead)
        OnRead(buffer_);
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
