#include "receive_tcpthread.h"

Receive_TcpThread::Receive_TcpThread()
{

}

Receive_TcpThread::~Receive_TcpThread()
{

}

void Receive_TcpThread::run()
{
    exec();
}

void Receive_TcpThread::SubOne()
{
    --sessionCount;
}

void Receive_TcpThread::AddOne()
{
    ++sessionCount;
}
