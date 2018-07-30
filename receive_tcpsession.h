/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：Receive_TcpSession.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 会话（连接）头文件
  * @date         作成日期: 2018-07-27 14:47:07 周五
  * @date         修正日期：2018-07-27 14:47:07 周五
  ---------------------------------------------------------
  * */
#ifndef RECEIVE_TCPSESSION_H
#define RECEIVE_TCPSESSION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <functional>

#include "receive_tcpthread.h"

class Receive_TcpSession : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Receive_TcpSession(Receive_TcpThread *parent = nullptr);
    ~Receive_TcpSession();

    void ConnectToServer(const QString &host, quint16 port);
    // 断开连接
    void Disconnect();
    void Write(const char *data, qint64 len);


public:
    // 断开连接回调
    std::function<void(void*)> OnDisConnected = nullptr;
    // 读数据回调
    std::function<void(const QByteArray&)> OnRead = nullptr;

signals:
    void SignalRead(const QByteArray &, int);
    void SignalDisConnected(void *);
    void SignalDoWrite();
    void SignalDoDisConnect();
    void SignalDoConnectToServer(const QString &, quint16);

private slots:
    // 连接Server
    void SlotDoConnectToServer(const QString &host, quint16 port);
    // 开始读数据
    void SlotStartRead();
    // 断开连接回调
    void SlotDisConnected();
    // 写数据
    void SlotDoWrite();
    // 断开连接
    void SlotDoDisconnect();

private:
    Receive_TcpThread *thread_ = nullptr;
    QByteArray buffer_ = nullptr;
    QByteArray writeBuffer_ = nullptr;
};

#endif // RECEIVE_TCPSESSION_H
