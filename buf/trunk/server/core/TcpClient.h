
#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "Config.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "Noncopyable.h"
#include "Thread.h"
#include "Event.h"

namespace buf
{

class TcpClient : public TcpConnection, public Thread , public Noncopyable
{
public:
    static const int TICK = 5;
public:
    TcpClient(const char* name, const char* ip, port_t port, bool cached = true)
        : TcpConnection(ip, port, cached), Thread(name) {}
    virtual ~TcpClient() {}

    virtual void run();
    virtual void onFinal()
    {
        // TODO: 必须要在这个函数里实现通知进程终止运行或者清理自己并调起~TcpClient()
    }
};

class TcpClientEvent : public Event
{
public:
    TcpClientEvent(socket_t s, event_t events, TcpClient* tc)
        : Event(s, events), _tc(tc) {}
    virtual ~TcpClientEvent()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
        _tc = 0;
    }

    virtual void onError();
    virtual void onEof();
    virtual bool onEvent(event_t ev);

private:
    TcpClient* _tc;
};

} // namespace buf

#endif // TCPCLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

