#ifndef SESSIONINFOLIST_H
#define SESSIONINFOLIST_H

#include <mutex>
#include <memory>
#include <vector>

#include "sessioninfo.h"

class SessionInfoList
{
public:
    SessionInfoList();
    ~SessionInfoList();

    SessionInfo* NewSessionInfo(std::shared_ptr<Receive_TcpSession> &session);
    void Clear();
    SessionInfo* FindByID(const void *id);

private:
    void SessionDisconnected(void *id);
private:
    std::mutex lock_;
    std::vector<SessionInfo*> list_;
};

#endif // SESSIONINFOLIST_H
