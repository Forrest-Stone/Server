#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include <QObject>
#include <memory>
#include <functional>

#include "receive_tcpsession.h"

class SessionInfo : public QObject
{
    Q_OBJECT  
public:
    SessionInfo(std::shared_ptr<Receive_TcpSession> &session);
    ~SessionInfo();

    //断开连接
    void Disconnect();

signals:
    void SignalRead(SessionInfo*, qint64 size);
    void SignalReadClient(SessionInfo*, QString);
    void SignalDisconnect();
    void SignalConnected();


public:
    //断开连接回调
    std::function<void(void*)> OnDisConnected = nullptr;

private slots:
    void SlotRead(qint64 size);
    void SlotReadClient(QString client);
    void SlotDisconnected();

private:
    std::shared_ptr<Receive_TcpSession> session_;
};

#endif // SESSIONINFO_H
