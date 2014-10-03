
#ifndef GAMECLIENT_H_
#define GAMECLIENT_H_

#include "core/TcpConnection.h"
#include "core/MsgQueue.h"
#include "core/Noncopyable.h"
#include "core/Logger.h"

namespace buf
{

class GameClient : public TcpConnection, public MessageQueue, public Noncopyable
{
public:
    GameClient(const char* ip, port_t port, s32_t type, bool cached = true)
        : TcpConnection(ip, port, cached), _type(type) {}

    virtual ~GameClient() {}

    inline s32_t getType() { return _type; }

    bool connect2(const char* sn)
    {
        if (!connect(sn, true))
            return false;
        return true;
    }

    virtual void onConnected()
    {
        if (connected())
        {
            // XXX: 修正建立连接的时间
            fixCreated();

            sLog.trace("ACK(%u) to %s:%u.", getType(), getIp(), getPort());
            if (forceSendPack(&_type, sizeof(_type)) < 0)
            {
                sLog.append(" [FAILED].\n");
                return;
            }
            sLog.append(" [SUCC].\n");
            return;
        }
    }

    // ACK message from GameServer, just the type of the GameServer
    virtual int ack(const void* pack, int len)
    {
        if (len < (int)sizeof(s32_t))
            return -1;
        s32_t type = *(s32_t*)(pack);
        if (type != SERVER_GAME)
            return -1;
        return 1;
    }

    virtual int process(const void* pack, int len)
    {
        bool ret = MessageQueue::pushCmd(pack, len);
        if (!ret)
        {
        }
        return 0;
    }

    virtual bool uniqueAdd() { return true; }
    virtual void uniqueRemove() {}

    virtual int recycle() { return 1; }

    virtual void heartbeat() {}
    virtual void onDisconnected() {}
    virtual void onEnterProcess() {}

private:
    s32_t _type;
};

} // namespace buf

#endif // GAMECLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

