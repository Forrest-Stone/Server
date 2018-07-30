#include "tcpthread.h"

TcpThread::TcpThread()
{

}

TcpThread::~TcpThread()
{

}

void TcpThread::run()
{
    exec();
}

void TcpThread::SubOne()
{
    --sessionCount;
}

void TcpThread::AddOne()
{
    ++sessionCount;
}
