#include "login_mythread.h"
#include <QHostAddress>
MyThread::MyThread(QObject *parent,set<MyTcpSocket*>&sockets,handler&fun) : QThread(parent),sockets(sockets),fun(fun)
{

}
void MyThread::add_socket(qintptr socketDescriptor)
{
    MyTcpSocket *socket = new MyTcpSocket(socketDescriptor,fun,sockets);
    sockets.insert(socket);
    socket->moveToThread(this);

}

void MyThread::run()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",QString::number((long long)currentThreadId()));
    db.setHostName("localhost");
    db.setDatabaseName("car");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("363677052");
    bool ok = db.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else
        qDebug() << "数据库连接失败";
    exec();
}
