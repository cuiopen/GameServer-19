
#ifndef TCPPROCESSOR_H_
#define TCPPROCESSOR_H_

#include "TcpProcess.h"
#include "Socket.h"
#include "Event.h"
#include "RTime.h"

namespace buf
{

class StateThread;

class TcpProcessor : public TcpProcess
{
public:
    TcpProcessor(socket_t s, const sockaddrin_t* sa, unsigned int id = 0)
        : TcpProcess(id), _sock(s, sa)
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }
    virtual ~TcpProcessor()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    // XXX: you may call recvPack after your owner recive function.
    // If the client is Flash, you may call Socket::recv first to
    // recive policy request and then Socket::send to send policy
    // content
    int recvPack(); // XXX: recv packet and call process
    int sendPack(const void* buf, int size);
    int recv();
    int send();

    virtual socket_t fd() const { return _sock.fd(); }
    virtual const Socket* sock() const { return &_sock; }

    // return value 1  - will be erased
    //              0  - must try again next time
    //              -1 - ERROR
    virtual int ack(const void* pack, int len)
    {
        // TODO:
        (void)pack;(void)len;
        printf("[%s] %s : %s\n", Time().hhmmss(), __PRETTY_FUNCTION__, (char*)pack);
        sendPack(pack, len);
        return 1;
    }

    virtual int sync()
    {
        return 1;
    }

    virtual int process(const void* pack, int len)
    {
        // XXX: should put pack into a message queue
        // TODO:
        (void)pack;(void)len;
        printf("[%s] %s : %s\n", Time().hhmmss(), __PRETTY_FUNCTION__, (char*)pack);
        sendPack(pack, len);
        return 0;
    }

    virtual int recycle()
    {
        return 1;
    }

    virtual void heartbeat()
    {
    }

    virtual void onConnected()
    {
    }

    virtual void onDisconnected()
    {
    }

    virtual void onEnterProcess()
    {
    }

private:
    Socket _sock;
};

class TcpProcessorAckEvent : public TcpProcessEvent
{
public:
    TcpProcessorAckEvent(socket_t s, event_t events, const TcpProcess* tp, const StateThread* st)
        : TcpProcessEvent(s, events, tp), _st(const_cast<StateThread*>(st)) {}
    virtual void onError();
    virtual void onEof();
    virtual bool onEvent(event_t ev);

private:
    StateThread* _st;
};

class TcpProcessorProcessingEvent : public TcpProcessEvent
{
public:
    TcpProcessorProcessingEvent(socket_t s, event_t events, const TcpProcess* tp, const StateThread* st)
        : TcpProcessEvent(s, events, tp), _st(const_cast<StateThread*>(st)) {}
    virtual void onError();
    virtual void onEof();
    virtual bool onEvent(event_t ev);

private:
    StateThread* _st;
};

} // namespace buf

#endif // TCPPROCESSOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

