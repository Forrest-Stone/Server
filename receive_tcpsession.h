﻿/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：Receive_TcpSession.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 会话（连接）头文件
  * @date         作成日期: 2018-07-27 14:47:07 周五
  * @date         修正日期：2018-07-27 14:47:07 周五
  ---------------------------------------------------------
  * */
#pragma execution_character_set("utf-8")
#ifndef RECEIVE_TCPSESSION_H
#define RECEIVE_TCPSESSION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <functional>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDataStream>
#include <QCoreApplication>

#include "receive_tcpthread.h"
#include "showpicture.h"

class Receive_TcpSession : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Receive_TcpSession(Receive_TcpThread *parent = nullptr);
    ~Receive_TcpSession();

    // 断开连接
    void Disconnect();
    void Write(const char *data, qint64 len);


public:
    // 断开连接回调
    std::function<void(void*)> OnDisConnected = nullptr;
    // 读数据回调
    std::function<void(const QByteArray&)> OnRead = nullptr;

    void processFileData(QByteArray &array);

signals:
    void SignalRead(qint64 size);
    void SignalDisConnected(void *);
    void SignalDoDisConnect();
    void SignalDoConnectToServer(const QString &, quint16);
    void SignalReadFileFinish();

    // 接收客户端有关信息
    void SignalClientIP(QString ip);

    // 接受文件有关信号
    void SignalReadFile(qint64 size);
    void SignalReadFileName(const QString name);
    void SignalReadFileSize(qint64 size);
    void SignalReadFilePath(QString path);

private slots:
    // 开始读数据
    void SlotStartRead();
    // 断开连接回调
    void SlotDisConnected();
    // 断开连接
    void SlotDoDisconnect();

    void ConnectToServer(const QString &host, quint16 port);
    void SlotDoConnectToServer(const QString &host, quint16 port);
    void SlotDisplayErrorMessage(QAbstractSocket::SocketError);

private:
    Receive_TcpThread *thread_ = nullptr;
    QByteArray buffer_ = nullptr;
    QByteArray writeBuffer_ = nullptr;

    // 接收文件有关信息
    QFile *receiveFile_;            // 待接收文件
    QString receiveFileName_;       // 待接收文件名
    QString receiveFilePath_;       // 待接收文件路径
    qint64 receiveFileTotalBytes_;  // 待接收文件总大小
    qint64 haveReceFileVytes_;      // 已经接收文件字节
    qint64 blockSize_;
    qint64 blockNumber_;
};

#endif // RECEIVE_TCPSESSION_H
