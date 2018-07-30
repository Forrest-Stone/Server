/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：receive_tcpthread.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 线程头文件
  * @date         作成日期: 2018-07-27 14:48:12 周五
  * @date         修正日期：2018-07-27 14:48:12 周五
  ---------------------------------------------------------
  * */
#ifndef RECEIVE_TCPTHREAD_H
#define RECEIVE_TCPTHREAD_H

#include <QThread>
#include <atomic>

class Receive_TcpThread : public QThread
{
    Q_OBJECT
public:
    Receive_TcpThread();
    ~Receive_TcpThread();

    virtual void run()override;

    void SubOne();
    void AddOne();
    //会话数
    std::atomic_uint32_t sessionCount = 0;
};

#endif // RECEIVE_TCPTHREAD_H
