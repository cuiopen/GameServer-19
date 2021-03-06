
#ifndef SESSIONPROCESSOR_H_
#define SESSIONPROCESSOR_H_

#include "core/TcpProcessor.h"
#include "core/MsgQueue.h"
#include "common/Servers.h"

namespace buf
{

class SessionServer;

class SessionProcessor : public TcpProcessor, public MessageQueue
{
public:
    SessionProcessor(SessionServer* ls, socket_t s, const sockaddrin_t* sa, unsigned int id = 0)
        : TcpProcessor(s, sa, id), _gm(false), _type(SERVER_UNKNOWN), _recycle(true), _ls(ls)
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    virtual ~SessionProcessor()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    inline void setType(s32_t type) { _type = type; }
    inline s32_t getType() const { return _type; }

    inline void setGM() { _gm = true; }
    inline bool isGM() const { return _gm; }

    virtual bool uniqueAdd();
    virtual void uniqueRemove();

    virtual int ack(const void* pack, int len);
    virtual int sync();
    virtual int process(const void* pack, int len);
    virtual int recycle();
    virtual void heartbeat();
    virtual void onConnected();
    virtual void onEnterProcess();

    virtual bool cmdProcessor(const void*, unsigned int);

private:
    bool _gm;
    s32_t _type;
    bool _recycle;
    SessionServer* _ls;
};

} // namespace buf

#endif // SESSIONPROCESSOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

