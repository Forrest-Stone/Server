#ifndef Login_TcpServer_H
#define Login_TcpServer_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
#include <QThread>
#include "login_mythread.h"
#include <handler.h>
#include "login_mytcpsocket.h"
using namespace std;
class Login_TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit Login_TcpServer(QObject *parent,int port,int thread_num,handler&fun);
    virtual ~Login_TcpServer();
private:
    vector<set<MyTcpSocket*>> sockets;
    vector<MyThread*> threads;
    handler&fun;
   // int cur_thread;
    int max_thread;
    int port;
    //int socket_num;

    MyThread *find_thread();
protected:
    virtual void incomingConnection(qintptr socketDescriptor);
signals:

public slots:
private slots:
   // void newConnectionSlot();
};

#endif // Login_TcpServer_H
