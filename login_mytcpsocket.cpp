#include "login_mytcpsocket.h"
#include <QHostAddress>
#include <QThread>
MyTcpSocket::MyTcpSocket(qintptr socketDescriptor, handler &fun,set<MyTcpSocket*>&sockets,QObject* parent):QTcpSocket(parent),fun(fun),sockets(sockets)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()),Qt::QueuedConnection);
    connect(this,SIGNAL(connected()),this,SLOT(showConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(disconnected()),this,SLOT(remove_set()),Qt::QueuedConnection);
}

void MyTcpSocket::readMessage()
{
    QTcpSocket*socket= static_cast<QTcpSocket*>(sender());
    fun(socket);
}
void MyTcpSocket::showConnected()
{
    MyTcpSocket*socket= static_cast<MyTcpSocket*>(sender());
    qDebug()<<socket->peerAddress().toString()<<"  connected!";
}
void MyTcpSocket::remove_set()
{
    MyTcpSocket*socket= static_cast<MyTcpSocket*>(sender());
    qDebug()<<socket->peerAddress().toString()<<"  disconnected!";
    sockets.erase(socket);
    disconnect(socket,SIGNAL(connected()),this,SLOT(showConnected()));
    disconnect(socket,SIGNAL(disconnected()),this,SLOT(remove_set()));
    socket->deleteLater();
    if(sockets.size()==0)
    {
        qDebug()<<"now thread "<<QThread::currentThreadId()<<" has no socket, so will stop.";
    }
}

