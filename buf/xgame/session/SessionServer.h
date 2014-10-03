
#ifndef SESSIONSERVER_H_
#define SESSIONSERVER_H_

#include "common/SlaveServer.h"

namespace buf
{

class SessionServer : public SlaveServer
{
public:
    SessionServer(const char* name);
    virtual ~SessionServer();

    virtual bool init();

    bool ipMask(const char* ip);
    
    inline bool ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info)
    {
        return true;
    }

    inline bool sync(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool process(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool recycle(s32_t type, s32_t id)
    {
        return true;
    }

    virtual TcpProcessor* createProcessor(socket_t s, sockaddrin_t* sa);

private:
};

} // namespace buf

#endif // SESSIONSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

