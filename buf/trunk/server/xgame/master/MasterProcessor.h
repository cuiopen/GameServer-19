
#ifndef MASTERPROCESSOR_H_
#define MASTERPROCESSOR_H_

#include "core/TcpProcessor.h"
#include "core/MsgQueue.h"
#include "common/Servers.h"

namespace buf
{

class MasterServer;

class MasterProcessor : public TcpProcessor, public MessageQueue
{
public:
    MasterProcessor(MasterServer* ms, socket_t s, const sockaddrin_t* sa, unsigned int id = 0)
        : TcpProcessor(s, sa, id), _type(SERVER_UNKNOWN), _serverid(0), _recycle(true), _process(false), _ms(ms)
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    virtual ~MasterProcessor()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    inline void setType(s32_t type) { _type = type; }
    inline s32_t getType() const { return _type; }
    inline void setServerId(s32_t id) { _serverid = id; }
    inline s32_t getServerId() const { return _serverid; }

    virtual bool uniqueAdd();
    virtual void uniqueRemove();

    virtual int ack(const void* pack, int len);
    virtual int sync();
    virtual int process(const void* pack, int len);
    virtual int recycle();
    virtual void heartbeat();
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onEnterProcess();

    virtual bool cmdProcessor(const void*, unsigned int);

private:
    s32_t _type;
    s32_t _serverid;
    bool _recycle;
    bool _process;
    MasterServer* _ms;
};

} // namespace buf

#endif // MASTERPROCESSOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

