#include "sessionthreads.h"

SessionThreads::SessionThreads(QObject *parent) : QObject(parent)
{

}

SessionThreads::~SessionThreads()
{
    this->Stop();
}

/**
  ---------------------------------------------------------
  * @file         File Name: sessionthread.cpp
  * @function     Function Name: Start
  * @brief        Description: 线程池启动函数
  * @date         Date: 2018-07-27 11:26:57 周五
  * @param        Parameter: threadNum 线程池线程数量
  * @return       Return Code: true
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
bool SessionThreads::Start(uint32_t threadNum)
{
    if(isRunning_) {
        return true;
    }
    for(uint32_t i = 0; i < threadNum; ++i) {
        TcpThread *thread = new TcpThread();
        threadList_.push_back(thread);
        thread->start();
    }
    isRunning_ = true;
    return true;
}

/**
  ---------------------------------------------------------
  * @file         File Name: sessionthread.cpp
  * @function     Function Name: Stop
  * @brief        Description: 关闭线程池中线程连接
  * @date         Date: 2018-07-27 11:43:47 周五
  * @param        Parameter: Tags
  * @return       Return Code: None
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void SessionThreads::Stop()
{
    if(isRunning_) {
        return ;
    }
//    QMutexLocker locker(&lock_);
    // 先关闭已有连接
    lock_guard<mutex> locker(this->lock_);
    unordered_map<void*, shared_ptr<TcpSession>>::iterator itor = sessionList_.begin();
    // 关闭连接
    for(itor = sessionList_.begin(); itor != sessionList_.end(); ++itor) {
        shared_ptr<TcpSession> session = itor->second;
        if(session.get()) {
            disconnect(session.get(), &TcpSession::SignalDisConnected,
                       this, &SessionThreads::SlotSessionDisConnected);
            session.get()->Disconnect();
        }
    }
    for(TcpThread *thread : this->threadList_) {
        thread->exit();
        thread->wait();
    }
    for(TcpThread *thread : this->threadList_) {
       delete thread;
    }
    this->threadList_.clear();
    this->sessionList_.clear();
    isRunning_ = false;
}

/**
  ---------------------------------------------------------
  * @file         File Name: sessionthread.cpp
  * @function     Function Name: PickMinThread
  * @brief        Description: 获取最小会话数线程
  * @date         Date: 2018-07-27 13:08:19 周五
  * @param        Parameter: Tags
  * @return       Return Code: TcpThread
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
TcpThread *SessionThreads::PickMinThread()
{
    TcpThread *thread = nullptr;
    uint32_t minCount = 0;
    for(TcpThread *tmpThread : this->threadList_)
    {
        uint32_t tmpCount = tmpThread->sessionCount;
        if(minCount == 0 || tmpCount < minCount)
        {
            minCount = tmpCount;
            thread = tmpThread;
        }
    }
    return thread;
}

/**
  ---------------------------------------------------------
  * @file         File Name: sessionthread.cpp
  * @function     Function Name: GetSessionSize
  * @brief        Description: 获取会话数量
  * @date         Date: 2018-07-27 13:12:24 周五
  * @param        Parameter: Tags
  * @return       Return Code: vector<uint32_t>
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
vector<uint32_t> SessionThreads::GetSessionSize() const
{
    vector<uint32_t> vec;
    for(TcpThread *thread : this->threadList_) {
        vec.push_back(thread->sessionCount);
    }
    return vec;
}

/**
  ---------------------------------------------------------
  * @file         File Name: sessionthread.cpp
  * @function     Function Name: CreateSession
  * @brief        Description: 添加一个会话
  * @date         Date: 2018-07-27 13:13:48 周五
  * @param        Parameter: handle 要处理的连接描述符
  * @return       Return Code: shared_ptr<TcpSession>
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void SessionThreads::AddSession(shared_ptr<TcpSession> &session)
{
    connect(session.get(), &TcpSession::SignalDisConnected,
            this, &SessionThreads::SlotSessionDisConnected,
            Qt::QueuedConnection);
    // 加锁
    lock_guard<mutex> locker(this->lock_);
    this->sessionList_[session.get()] = session;
}

// 会话断开
void SessionThreads::SlotSessionDisConnected(void *id)
{
    // 加锁
    lock_guard<mutex> locker(this->lock_);
    unordered_map<void*, shared_ptr<TcpSession>>::iterator itor = sessionList_.begin();
    itor = sessionList_.find(id);
    if(itor != sessionList_.end())
    {
        sessionList_.erase(itor);
    }
}
