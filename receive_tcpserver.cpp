/**
  ---------------------------------------------------------
  * @projectName  项目名：Server
  * @file         文件名：tcpserver.cpp
  * @author       著作权所有者: 张岩森
  * @brief        内容: 服务器端主程序
  * @date         作成日期: 2018-07-27 14:37:01 周五
  * @date         修正日期：2018-07-27 14:37:01 周五
  ---------------------------------------------------------
  * */
#pragma execution_character_set("utf-8")
#include "receive_tcpserver.h"

Receive_TcpServer::Receive_TcpServer(QObject *parent) : QTcpServer(parent)
{

}

Receive_TcpServer::~Receive_TcpServer()
{
    this->Stop();
}
/**
  ---------------------------------------------------------
  * @file         File Name: tcpserver.cpp
  * @function     Function Name: incommingConnection
  * @brief        Description: 重写接收 TCP 请求的虚函数
  *                            实现多线程接收数据
  * @date         Date: 2018-07-27 14:34:33 周五
  * @param        Parameter: Tags
  * @return       Return Code: ReturnType
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void Receive_TcpServer::incomingConnection(qintptr socketDescriptor)
{
    shared_ptr<Receive_TcpSession> session = this->CreateSession(socketDescriptor);
    qDebug() << "TcpServer::incomingConnection socketDescriptor:"<< socketDescriptor ;
    if(this->OnAccepted) {
        this->OnAccepted(session);
    }
}

/**
  ---------------------------------------------------------
  * @file         File Name: tcpserver.cpp
  * @function     Function Name: Start
  * @brief        Description: 启动服务器
  * @date         Date: 2018-07-27 11:07:58 周五
  * @param        Parameter: portNum 端口号
  *                          threadNum 线程数量
  * @return       Return Code: None
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
bool Receive_TcpServer::Start(ServerData &conf)
{
    if(isRunning_) {
        return true;
    }
    //验证数据
    conf.Verify();
    //启动线程池
    sessionThreads_.Start(conf.threadNum);
    //监听端口
    if(!this->listen(QHostAddress::Any, (quint16)conf.portNum)) {
        return false;
    }
    qDebug() << "监听成功！" << endl;
    isRunning_ = true;
    return true;
}

/**
  ---------------------------------------------------------
  * @file         File Name: tcpserver.cpp
  * @function     Function Name: Stop
  * @brief        Description: 关闭服务器
  * @date         Date: 2018-07-27 12:51:57 周五
  * @param        Parameter: Tags
  * @return       Return Code: None
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
void Receive_TcpServer::Stop()
{
    if(!isRunning_) {
        return ;
    }
    //关闭监听
    this->close();
    //关闭线程池
    sessionThreads_.Stop();
    isRunning_ = false;
}

bool Receive_TcpServer::IsStart()
{
    return isRunning_;
}


/**
  ---------------------------------------------------------
  * @file         File Name: tcpserver.cpp
  * @function     Function Name: GetSessionSize
  * @brief        Description: 获得会话数量
  * @date         Date: 2018-07-27 13:00:50 周五
  * @param        Parameter: Tags
  * @return       Return Code: vector<unit32_t>
  * @author       Author: 张岩森
  ---------------------------------------------------------
  * */
vector<uint32_t> Receive_TcpServer::GetSessionSize() const
{
    return this->sessionThreads_.GetSessionSize();
}


shared_ptr<Receive_TcpSession> Receive_TcpServer::CreateSession(qintptr handle)
{
    Receive_TcpThread *thread = this->sessionThreads_.PickMinThread();
    shared_ptr<Receive_TcpSession> session = make_shared<Receive_TcpSession>(thread);
    session->setSocketDescriptor(handle);
    this->sessionThreads_.AddSession(session);
    session->moveToThread(thread);
    if(thread)
        thread->AddOne();
    return session;
}
