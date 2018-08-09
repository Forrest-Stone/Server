#include "sessioninfo.h"

SessionInfo::SessionInfo(std::shared_ptr<Receive_TcpSession> &session)
{
    this->session_ = session;
    connect(this->session_.get(), &Receive_TcpSession::disconnected,
            this, &SessionInfo::SlotDisconnected);
    connect(this->session_.get(), &Receive_TcpSession::SignalRead,
            this, &SessionInfo::SlotRead);
    connect(this->session_.get(), &Receive_TcpSession::connected,
            this, &SessionInfo::SignalConnected);
    connect(this->session_.get(), &Receive_TcpSession::SignalClientIP,
            this, &SessionInfo::SlotReadClient);
    connect(this->session_.get(), &Receive_TcpSession::SignalReadFileName,
            this, &SessionInfo::SlotReadFileName);
    connect(this->session_.get(), &Receive_TcpSession::SignalReadFilePath,
            this, &SessionInfo::SlotReadFilePath);
    connect(this->session_.get(), &Receive_TcpSession::SignalReadFileSize,
            this, &SessionInfo::SlotReadFileSize);
    connect(this->session_.get(), &Receive_TcpSession::SignalReadFileFinish,
            this, &SessionInfo::SlotReadFinish);
}

SessionInfo::~SessionInfo()
{
    if(!this->session_) {
        return ;
    }
    disconnect(this->session_.get(), &Receive_TcpSession::disconnected,
               this, &SessionInfo::SlotDisconnected);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalRead,
               this, &SessionInfo::SlotRead);
    disconnect(this->session_.get(), &Receive_TcpSession::connected,
               this, &SessionInfo::SignalConnected);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalClientIP,
               this, &SessionInfo::SlotReadClient);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalReadFileName,
               this, &SessionInfo::SlotReadFileName);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalReadFilePath,
               this, &SessionInfo::SlotReadFilePath);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalReadFileSize,
               this, &SessionInfo::SlotReadFileSize);
    disconnect(this->session_.get(), &Receive_TcpSession::SignalReadFileFinish,
               this, &SessionInfo::SlotReadFinish);
    this->session_ = nullptr;
}

void SessionInfo::Disconnect()
{
    if(this->session_) {
        this->session_->Disconnect();
    }
}

void SessionInfo::SlotRead(qint64 size)
{
    emit this->SignalRead(size);
}

void SessionInfo::SlotReadClient(QString client)
{
    emit this->SignalReadClient(client);
}

void SessionInfo::SlotReadFileName(QString fileName)
{
    emit this->SignalReadFileName(fileName);
}

void SessionInfo::SlotReadFilePath(QString path)
{
    emit this->SignalReadFilePath(path);
}

void SessionInfo::SlotReadFileSize(qint64 size)
{
    emit this->SignalReadFileSize(size);
}

void SessionInfo::SlotDisconnected()
{
    emit this->SignalDisconnect();
    if(this->OnDisConnected)
        this->OnDisConnected(this);
}

void SessionInfo::SlotReadFinish()
{
    emit this->SignalReadFinish();
}
