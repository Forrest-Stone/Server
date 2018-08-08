#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QTcpSocket>
#include <set>
#include "handler.h"
#include "login_mytcpsocket.h"
using namespace std;
/**
 * @brief The MyThread class
 * 因为QThread调用exec()后，就进入了事件驱动机制，因此没有信号的时候，他是不会执行占用CPU资源的，所以这就达成了我的目标的，不用我手动的去暂停
 * 没有socket要处理的线程
 **/
class MyThread :public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent,set<MyTcpSocket*>&sockets,handler&fun);
    void add_socket(qintptr socketDescriptor);
private:
    handler&fun;
    set<MyTcpSocket*>&sockets;
signals:

public slots:
protected:
    void run();

};

#endif // MYTHREAD_H
