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
    this->session_ = nullptr;
}

void SessionInfo::Connect(const QString &host, quint16 port)
{
    if(this->session_) {
        this->session_->ConnectToServer(host, port);
    }
}

void SessionInfo::Disconnect()
{
    if(this->session_) {
        this->session_->Disconnect();
    }
}

void SessionInfo::Write(const char *buffer, int size)
{
    if(this->session_) {
        this->session_->Write(buffer, size);
    }
}

void SessionInfo::SlotRead(qint64 size)
{
    emit this->SignalRead(this, size);
}

void SessionInfo::SlotReadClient(QString client)
{
    emit this->SignalReadClient(this, client);
}

void SessionInfo::SlotDisconnected()
{
    emit this->SignalDisconnect();
    if(this->OnDisConnected)
        this->OnDisConnected(this);
}
