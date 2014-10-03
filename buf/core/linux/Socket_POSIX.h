
#ifndef SOCKET_POSIX_H_
#define SOCKET_POSIX_H_

#include "../Config.h"
#include <sys/socket.h> // for sockaddr_in, sockaddr
#include <arpa/inet.h> // for inet_ntoa
#include <netinet/tcp.h> // for TCP_NODELAY
#include <sys/fcntl.h> // for fcntl

namespace buf
{

typedef int socket_t;
typedef unsigned short port_t;
typedef struct sockaddr_in sockaddrin_t;
typedef struct sockaddr sockaddr_t;

class SocketImpl
{
public:
    static const size_t SOCKIMPL_BUFFER_RECV = RECV_BUF_SIZE;
    static const size_t SOCKIMPL_BUFFER_SEND = SEND_BUF_SIZE;

    static const int    SOCKIMPL_RD_TIMEOUT = 3000;    // msecs
    static const int    SOCKIMPL_WR_TIMEOUT = 6000;    // msesc

public:
    static const char* getIpByIfImpl(const char* inet);

    inline static void setSendBufSizeImpl(socket_t s, int size)
    {
        setsockopt(s, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    }

    inline static void setRecvBufSizeImpl(socket_t s, int size)
    {
        setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    }

    inline static void setReuseImpl(socket_t s, bool reuse = true)
    {
        int flag = reuse?1:0;
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    }

    inline static void setNoDelayImpl(socket_t s, bool nodelay = true)
    {
        // XXX: disable Nagle
        int flag = nodelay?1:0;
        setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    }

    static void setNonblockImpl(socket_t s, bool nonblock = true)
    {
        int flag = fcntl(s, F_GETFL, 0);
        if (flag < 0)
            return;
        if (nonblock) {
            flag |= O_NONBLOCK;
            ::fcntl(s, F_SETFL, flag);
        } else {
            flag &= ~O_NONBLOCK;
            ::fcntl(s, F_SETFL, flag);
        }
    }

    inline static void setBlockImpl(socket_t s)
    {
        return setNonblockImpl(s, false);
    }

    inline static int addr2netImpl(const char* ip) { return inet_addr(ip); }
    inline static const char* net2addrImpl(int ip) { struct in_addr addr; addr.s_addr = ip; return inet_ntoa(addr); }
    inline static u32_t toNetLongImpl(u32_t hl) { return htonl(hl); }
    inline static u16_t toNetShortImpl(u16_t hs) { return htons(hs); }
    inline static u32_t toHostLongImpl(u32_t nl) { return ntohl(nl); }
    inline static u16_t toHostShortImpl(u16_t ns) { return ntohs(ns); }

public:
    SocketImpl(socket_t s, const sockaddrin_t* sa);
    ~SocketImpl();

    socket_t fdImpl() const { return _sock; }
    int recvImpl(void* buf, int size);
    int syncRecvImpl(void* buf, int size);

    int sendImpl(const void* buf, int size);
    int syncSendImpl(const void* buf, int size);

    inline bool incompleterImpl() const { return _incompleter; }
    inline bool incompletewImpl() const { return _incompletew; }

    inline const char* getLocalIpImpl() const { return inet_ntoa(_lsa.sin_addr); }
    inline unsigned short getLocalPortImpl() const { return ntohs(_lsa.sin_port); }
    inline const char* getRemoteIpImpl() const { return inet_ntoa(_rsa.sin_addr); }
    inline unsigned short getRemotePortImpl() const { return ntohs(_rsa.sin_port); }
    inline int getRemoteIpRawImpl() const { return _rsa.sin_addr.s_addr; }

    inline void setSendBufSizeImpl(int size) { setSendBufSizeImpl(_sock, size); }
    inline void setRecvBufSizeImpl(int size) { setRecvBufSizeImpl(_sock, size); }
    inline void setReuseImpl(bool reuse = true) { setReuseImpl(_sock, reuse); }
    inline void setNoDelayImpl(bool nodelay = true) { setNoDelayImpl(_sock, nodelay); } /* XXX: disable Nagle*/
    inline void setNonblockImpl(bool nonblock = true) { setNonblockImpl(_sock, nonblock); }

    inline bool isPeerClosed() const { return _peerclosed || _sock < 0; }
    inline bool closedImpl() const { return _peerclosed || _sock < 0; }
    inline bool connectedImpl() const { return !closedImpl(); }
    inline bool disconnectedImpl() const { return closedImpl(); }

    void closeImpl();

private:
    int wait4send(int fd);
    int wait4read(int fd);

private:
    socket_t _sock;
    sockaddrin_t _rsa; // remote sockaddr
    sockaddrin_t _lsa; // local sockaddr
    bool _incompleter;
    bool _incompletew;
    bool _peerclosed;
};

} // namespace buf

#endif // SOCKET_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

