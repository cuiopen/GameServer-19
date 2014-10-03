
#ifndef TCPSERVER_POSIX_H_
#define TCPSERVER_POSIX_H_

#include "../Socket.h"

namespace buf
{

class TcpServerImpl
{
public:
    TcpServerImpl() : _retry(false) {}
    ~TcpServerImpl() {}

    bool bindImpl(const char* ip, port_t port, int backlog, socket_t& ns/*OUT*/);
    bool acceptImpl(socket_t s, sockaddrin_t* sa, socklen_t len, socket_t& ns/*OUT*/);
    bool retryImpl() const { return _retry; }

private:
    bool _retry;
};

} // namespace buf

#endif // TCPSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

