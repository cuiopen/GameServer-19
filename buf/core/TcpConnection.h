
#ifndef TCPCONN_H_
#define TCPCONN_H_

#include "Config.h"
#include "System.h"
#ifdef UNIXLIKE
#include "linux/TcpConnection_POSIX.h"
#endif
#include "Socket.h"
#include "TcpProcess.h"
#include "StateThread.h"
#include "RTime.h"
#include "Utils.h"

#include <string>

namespace buf
{

class TcpConnection : public TcpConnectionImpl, public TcpProcess
{
public:
    TcpConnection(const char* ip, port_t port, bool cached = true)
        : _sock(0), _port(port?port:PORT_ANY), _reconnect(false)
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
        cpystr(ip?ip:HOST_ANY, _ip, sizeof(_ip));
        setCached(cached);
    }
    virtual ~TcpConnection()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
        if (_sock)
            DELETENULL(_sock);
    }

    void setZip(bool onoff = true)
    {
        if (_sock)
            _sock->setZip(onoff);
    }

    void setZipOn()
    {
        if (_sock)
            _sock->setZip(true);
    }

    void setZipOff()
    {
        if (_sock)
            _sock->setZip(false);
    }

    void setEncrypt(int type)
    {
        if (_sock)
            _sock->setEncrypt(type);
    }

    inline bool isReconnect() const { return _reconnect; }
    inline bool getReconnect() const { return _reconnect; }
    inline void setReconnect(bool v) { _reconnect = v; }

    bool connect(const char* sn = NULL, bool reconnect = false);
    bool connect(const char* ip, port_t port);
    inline void close()
    {
        if (_sock)
            _sock->close();
    }

    inline bool connected() const { return _sock && _sock->connected(); }
    inline bool disconnected() const { return !_sock || _sock->disconnected(); }
    inline bool closed() const { return !_sock || _sock->closed(); }

    const char* getIp() const { return _ip; }
    unsigned short getPort() const { return _port; }

    int send(const void* buf, int size);
    int recv(void* buf, int size);
    int syncRecv(void* buf, int size);

    int sendPack(const void* buf, int size);
    int forceSendPack(const void* buf, int size);
    int recvPack(void* buf, int size);
    int recvPack();

    inline socket_t fd() const { return _sock?_sock->fd():-1; }
    inline Socket* sock() { return _sock; }
    inline const Socket* sock() const { return _sock; }

    // XXX: You should to fix prombles with sync by yourself
    
    // XXX: 在建立好连接之后必须要收到服务器的 ACK 指令后才能继续接收消息
    virtual int ack(const void* pack, int len)
    {
        (void)pack;(void)len;
        printf("[%s] %s : %s\n", Time().hhmmss(), __PRETTY_FUNCTION__, (char*)pack);
        sendPack(pack, len);
        return 1;
    }

    virtual int sync() { return 1; } // XXX: no need to sync when TcpProcess is a client

    virtual int process(const void* pack, int len)
    {
        (void)pack;(void)len;
        printf("[%s] %s : %s\n", Time().hhmmss(), __PRETTY_FUNCTION__, (char*)pack);
        sendPack(pack, len);
        return 1;
    } // exchange messages

    virtual int recycle() { return 1; }

    virtual void heartbeat() {}
    virtual void onConnected() {}
    virtual void onDisconnected() {}
    virtual void onEnterProcess() {}

private:
    Socket* _sock;
    char _ip[32];
    port_t _port;
    bool _reconnect;
};

class TcpConnectionAckEvent : public TcpProcessEvent
{
public:
    TcpConnectionAckEvent(socket_t s, event_t events, const TcpProcess* tp, const StateThread* st)
        : TcpProcessEvent(s, events, tp), _st(const_cast<StateThread*>(st)) {}
    virtual void onError();
    virtual void onEof();
    virtual bool onEvent(event_t ev);

private:
    StateThread* _st;
};

class TcpConnectionProcessingEvent : public TcpProcessEvent
{
public:
    TcpConnectionProcessingEvent(socket_t s, event_t events, const TcpProcess* tp, const StateThread* st)
        : TcpProcessEvent(s, events, tp), _st(const_cast<StateThread*>(st)) {}
    virtual void onError();
    virtual void onEof();
    virtual bool onEvent(event_t ev);

private:
    StateThread* _st;
};

}

#endif // TCPCONN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

