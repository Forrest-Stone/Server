/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         sessionthread.h
  * @author       著作权所有者: 张岩森
  * @brief        内容: 重新设计多线程头文件
  * @date         作成日期: 2018-07-27 14:46:16 周五
  * @date         修正日期：2018-07-27 14:46:16 周五
  ---------------------------------------------------------
  * */
#ifndef SESSIONTHREADS_H
#define SESSIONTHREADS_H

#include <QObject>
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <memory>
#include <mutex>
#include <QMutex>
#include <QMutexLocker>

#include "tcpthread.h"
#include "tcpsession.h"

using namespace std;

class SessionThreads : public QObject
{
    Q_OBJECT
public:
    explicit SessionThreads(QObject *parent = nullptr);
    ~SessionThreads();

    //启动线程池, 非线程安全
    bool Start(uint32_t threadNum);
    //关闭, 非线程安全
    void Stop();
    //获取最小会话数线程
    TcpThread *PickMinThread();
    //获取会话数
    vector<uint32_t> GetSessionSize()const;
    //添加会话
    void AddSession(std::shared_ptr<TcpSession> &session);

private slots:
    //会话断开
    void SlotSessionDisConnected(void *id);

private:
    vector<TcpThread*> threadList_;
    //互斥量
    mutex lock_;
    //会话列表
    unordered_map<void*, shared_ptr<TcpSession>> sessionList_;

    bool isRunning_ = false;
};

#endif // SESSIONTHREADS_H
