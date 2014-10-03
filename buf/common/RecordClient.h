
#ifndef RECORDCLIENT_H_
#define RECORDCLIENT_H_

#include "BufferClient.h"
#include "CommonClient.h"

namespace buf
{

template <typename T = CommonClient>
class RecordClient : public T
{
public:
    RecordClient(const char* name, const char* ip, port_t port, s32_t type)
        : T(name, ip, port, type)
    {
        // T::setReconnect(true);
    }

    virtual ~RecordClient() {}

#if 0
    // XXX: 只有T=BufferClient时cmdProcessor才起作用
    //      子类必须实现这个函数
    virtual bool cmdProcessor(const void*, unsigned int)
    {
        return false;
    }

    virtual int ack(const void* pack, int len);
    virtual int process(const void* pack, int len);

    virtual void heartbeat();
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onEnterProcess();
#endif
};

} // namespace buf

#endif // RECORDCLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

