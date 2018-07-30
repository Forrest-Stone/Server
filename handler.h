#ifndef HANDLER_H
#define HANDLER_H
#include <QTcpSocket>

class handler
{
public:
    handler();
    virtual void operator()(QTcpSocket*)=0;
};

#endif // HANDLER_H
