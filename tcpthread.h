/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：tcpthread.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 线程头文件
  * @date         作成日期: 2018-07-27 14:48:12 周五
  * @date         修正日期：2018-07-27 14:48:12 周五
  ---------------------------------------------------------
  * */
#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <atomic>

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread();
    ~TcpThread();

    virtual void run()override;

    void SubOne();
    void AddOne();
    //会话数
    std::atomic_uint32_t sessionCount = 0;
};

#endif // TCPTHREAD_H
