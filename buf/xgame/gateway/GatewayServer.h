
#ifndef GATEWAYSERVER_H_
#define GATEWAYSERVER_H_

#include "common/SlaveServer.h"

namespace buf
{

class GatewayServer : public SlaveServer
{
public:
    GatewayServer(const char* name);
    virtual ~GatewayServer();

    virtual bool init();
    bool initGameClients();

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

#endif // GATEWAYSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

