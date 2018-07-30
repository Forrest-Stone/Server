#include "login_mythread.h"
#include <QHostAddress>
MyThread::MyThread(QObject *parent,set<QTcpSocket*>&sockets,handler&fun) : QThread(parent),sockets(sockets),fun(fun)
{

}
void MyThread::add_socket(QTcpSocket*socket)
{
    sockets.insert(socket);
    //connect(socket,SIGNAL(connected()),this,SLOT(showConnected()),Qt::QueuedConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(remove_set()),Qt::QueuedConnection);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()),Qt::QueuedConnection);

}
void MyThread::readMessage()
{
    QTcpSocket*socket= static_cast<QTcpSocket*>(sender());
    fun(socket);
}
void MyThread::showConnected()
{
    QTcpSocket*socket= static_cast<QTcpSocket*>(sender());
    qDebug()<<socket->peerAddress().toString()<<"  connected!";
}
void MyThread::remove_set()
{
    QTcpSocket*socket= static_cast<QTcpSocket*>(sender());
    qDebug()<<socket->peerAddress().toString()<<"  disconnected!";
    sockets.erase(socket);
    disconnect(socket,SIGNAL(disconnected()),this,SLOT(remove_set()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    socket->deleteLater();
    if(sockets.size()==0)
    {
        qDebug()<<"now thread "<<this->currentThreadId()<<" has no socket, so will stop.";
    }
}

