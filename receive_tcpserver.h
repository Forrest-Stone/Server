/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：tcpserver.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 服务器端头文件
  * @date         作成日期: 2018-07-27 14:46:43 周五
  * @date         修正日期：2018-07-27 14:46:43 周五
  ---------------------------------------------------------
  * */
#ifndef RECEIVE_TCPSERVER_H
#define RECEIVE_TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <QHostAddress>

#include "sessionthreads.h"
#include "receive_tcpsession.h"

#define PORTNUM 8888

//Server数据格式
struct ServerData
{
    uint16_t portNum;
    uint32_t threadNum;
    void Verify()
    {
        if(0 == portNum) {
            portNum = PORTNUM;
        }
        if(!threadNum) {
            // 硬件线程上下文数量
            threadNum = thread::hardware_concurrency();
        }
    }
};

class Receive_TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit Receive_TcpServer(QObject *parent = nullptr);
    ~Receive_TcpServer();

    // 启动服务器
    bool Start(ServerData &conf);
    // 关闭服务器
    void Stop();

    bool IsStart();
    // 获取会话数量
    vector<uint32_t> GetSessionSize() const;

public:
    // 新连接回调 通知上层 并把新连接丢给上层
    function<void(shared_ptr<Receive_TcpSession> &)> OnAccepted = nullptr;

protected:
    // 虚函数 接收 TCP 请求
    virtual void incomingConnection(qintptr socketDescriptor);
    // 创建会话
    shared_ptr<Receive_TcpSession> CreateSession(qintptr handle);

private:
    bool isRunning_ = false;
    SessionThreads sessionThreads_;
};

#endif // RECEIVE_TCPSERVER_H
