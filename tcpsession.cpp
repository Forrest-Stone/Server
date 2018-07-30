/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：tcpsession.cpp
  * @author       著作权所有者: 张岩森
  * @brief        内容: 会话源程序，主要处理函数
  * @date         作成日期: 2018-07-27 14:49:34 周五
  * @date         修正日期：2018-07-27 14:49:34 周五
  ---------------------------------------------------------
  * */
#include "tcpsession.h"

/**
  ---------------------------------------------------------
  * @file         File Name: tcpsession.cpp
  * @function     Function Name: TcpSession
  * @brief        Description: 构造函数，初始化网络连接信号
  * @date         Date: 2018-07-27 14:50:32 周五
  * @param        Parameter: parent 线程类型
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
TcpSession::TcpSession(TcpThread *parent)
{
    this->thread_ = parent;
    connect(this, &TcpSession::readyRead,
            this, &TcpSession::SlotStartRead);
    connect(this, &TcpSession::disconnected,
            this, &TcpSession::SlotDisConnected);
    connect(this, &TcpSession::SignalDoDisConnect,
            this, &TcpSession::SlotDoDisconnect);
    connect(this, &TcpSession::SignalDoWrite,
            this, &TcpSession::SlotDoWrite);
    connect(this, &TcpSession::SignalDoConnectToServer,
            this, &TcpSession::SlotDoConnectToServer);
}

/**
  ---------------------------------------------------------
  * @file         File Name: tcpsession.cpp
  * @function     Function Name: ~TcpSession
  * @brief        Description: 析构函数，处理断开连接的会话
  * @date         Date: 2018-07-27 14:52:46 周五
  * @param        Parameter: Tags
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
TcpSession::~TcpSession()
{
    disconnect(this, &TcpSession::readyRead,
               this, &TcpSession::SlotStartRead);
    disconnect(this, &TcpSession::disconnected,
               this, &TcpSession::SlotDisConnected);
    disconnect(this, &TcpSession::SignalDoDisConnect,
            this, &TcpSession::SlotDoDisconnect);
    disconnect(this, &TcpSession::SignalDoWrite,
               this, &TcpSession::SlotDoWrite);
    disconnect(this, &TcpSession::SignalDoConnectToServer,
            this, &TcpSession::SlotDoConnectToServer);
}

void TcpSession::ConnectToServer(const QString &host, quint16 port)
{
    emit this->SignalDoConnectToServer(host, port);
}

// 断开连接
void TcpSession::Disconnect()
{
    emit this->SignalDoDisConnect();
}

void TcpSession::Write(const char *data, qint64 len)
{
    writeBuffer_ = QByteArray(data, len);
    emit this->SignalDoWrite();
}

void TcpSession::SlotDoConnectToServer(const QString &host, quint16 port)
{
    this->connectToHost(QHostAddress(host), port);
}

void TcpSession::SlotStartRead()
{
    buffer_ = this->readAll();
    if(OnRead)
        OnRead(buffer_);
    emit this->SignalRead(buffer_.toStdString().c_str(), buffer_.length());
}

void TcpSession::SlotDisConnected()
{
    if(thread_)
        thread_->SubOne();
    //通知会话断开连接
    if(OnDisConnected)
        OnDisConnected(this);
    emit this->SignalDisConnected(this);
}

void TcpSession::SlotDoWrite()
{
    this->write(writeBuffer_);
}

void TcpSession::SlotDoDisconnect()
{
    this->disconnectFromHost();
}
