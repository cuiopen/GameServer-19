
#ifndef MASTERCLIENT_H_
#define MASTERCLIENT_H_

#include "CommonClient.h"

namespace buf
{

class MasterClient : public CommonClient
{
public:
    MasterClient(const char* name, const char* ip, port_t port, s32_t type)
        : CommonClient(name, ip, port, type) {}
    virtual ~MasterClient() {}

    virtual int ack(const void* pack, int len);
    virtual int process(const void* pack, int len);

    virtual void heartbeat();
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onEnterProcess();

private:

};

} // namespace buf

#endif // MASTERCLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

