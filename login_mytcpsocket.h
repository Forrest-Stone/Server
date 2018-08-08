#ifndef LOGIN_MYTCPSOCKET_H
#define LOGIN_MYTCPSOCKET_H
#include <QTcpSocket>
#include <set>
#include "handler.h"
using namespace std;
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket(qintptr socketDescriptor,handler&fun,set<MyTcpSocket*>&sockets,QObject*parent=nullptr);
private:
    handler&fun;
    set<MyTcpSocket*>&sockets;
private slots:
    void readMessage();
    void showConnected();
    void remove_set();
};

#endif // LOGIN_MYTCPSOCKET_H
