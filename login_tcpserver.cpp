#include "login_tcpserver.h"
#pragma execution_character_set("utf-8")
Login_TcpServer::~Login_TcpServer()
{
    for(int i=0;i<max_thread;i++)
    {
        if(threads[i]!=NULL)
        {
            threads[i]->quit();
            threads[i]->wait();
        }
    }
}
Login_TcpServer::Login_TcpServer(QObject *parent,int port,int thread_num,handler&fun):QTcpServer(parent),port(port),fun(fun),threads(thread_num,NULL)
{
    //cur_thread=0;
    max_thread=thread_num;
    //socket_num=0;
    for(int i=0;i<thread_num;i++)
    {
        set<QTcpSocket*> s;
        sockets.push_back(s);
    }
    this->setMaxPendingConnections(thread_num*10);
    this->listen(QHostAddress::Any,port);
    connect(this,SIGNAL(newConnection()),this,SLOT(newConnectionSlot()));
}
MyThread *Login_TcpServer::find_thread()
{
    for(int i=0;i<max_thread;i++)
    {
        if(sockets[i].size()<10)
        {
            if(threads[i]==NULL)
            {
                threads[i]=new MyThread(this,sockets[i],fun);
                threads[i]->start();
            }
            return threads[i];
        }
    }
    return NULL;
}
void Login_TcpServer::newConnectionSlot()
{
    QTcpSocket*socket=this->nextPendingConnection();
    qDebug()<<socket->peerAddress().toString()<<"  connected!";
    MyThread*thread=find_thread();
    if(thread==NULL)
    {
        qDebug()<<"没有空闲线程，连接失败！";
        socket->disconnectFromHost();
        return;
    }
    thread->add_socket(socket);

}
