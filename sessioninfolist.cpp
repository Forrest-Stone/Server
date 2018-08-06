#include "sessioninfolist.h"

SessionInfoList::SessionInfoList()
{

}

SessionInfoList::~SessionInfoList()
{
    this->Clear();
}

SessionInfo *SessionInfoList::NewSessionInfo(std::shared_ptr<Receive_TcpSession> &session)
{
    std::lock_guard<std::mutex> lock(this->lock_);
    SessionInfo *info = new SessionInfo(session);
    this->list_.push_back(info);
    info->OnDisConnected = std::bind(&SessionInfoList::SessionDisconnected,
                                     this, std::placeholders::_1);
    return info;
}

void SessionInfoList::Clear()
{
    std::lock_guard<std::mutex> lock(lock_);
    for(SessionInfo* info : this->list_)
    {
        if(info)
            delete info;
    }
    this->list_.clear();
}

SessionInfo *SessionInfoList::FindByID(const void *id)
{
    if (!id)
        return nullptr;
     std::lock_guard<std::mutex> lock(this->lock_);
     for (size_t i = 0; i < this->list_.size(); ++i)
     {
         SessionInfo *info = this->list_[i];
         if (info == id)
             return info;
     }
     return nullptr;
}

void SessionInfoList::SessionDisconnected(void *id)
{
    SessionInfo *info = nullptr;
    std::lock_guard<std::mutex> lock(this->lock_);
    // 将 session 从列表中删除
    std::vector<SessionInfo*>::iterator it;
    for (it = this->list_.begin(); it != this->list_.end(); ++it)
    {
        info = *it;
        if (info == id)
        {
            it = this->list_.erase(it);
            info->OnDisConnected = nullptr;
            delete info;
            return;
        }
    }
}
