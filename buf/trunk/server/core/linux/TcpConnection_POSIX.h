
#ifndef TCPCONNECTION_POSIX_H_
#define TCPCONNECTION_POSIX_H_

#include "../Socket.h"

namespace buf
{

class TcpConnectionImpl
{
public:
    TcpConnectionImpl() {}
    ~TcpConnectionImpl() {}
    Socket* connectImpl(const char* ip, port_t port);
};

} // namespace buf

#endif // TCPCONNECTION_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

