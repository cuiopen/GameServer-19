
#ifndef BUFFERCLIENT_H_
#define BUFFERCLIENT_H_

#include "CommonClient.h"
#include "core/MsgQueue.h"

namespace buf
{

class BufferClient : public CommonClient, public MessageQueue
{
public:
    BufferClient(const char* name, const char* ip, port_t port, s32_t type)
        : CommonClient(name, ip, port, type) {}

    virtual ~BufferClient() {}
};

} // namespace buf

#endif // BUFFERCLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

