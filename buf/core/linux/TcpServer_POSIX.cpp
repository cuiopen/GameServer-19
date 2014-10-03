
#include "TcpServer_POSIX.h"
#include <unistd.h> // for TEMP_FAILURE_RETRY
#include <errno.h> // for errno, EINTR
#include <sys/socket.h> // for set,AF_INET,inet*

namespace buf
{

bool TcpServerImpl::bindImpl(const char* ip, port_t port, int backlog, socket_t& ns)
{
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return false;

    Socket::setReuse(s, true);
    Socket::setNoDelay(s, true);
    Socket::setNonblock(s, true);
    Socket::setRecvBufSize(s, Socket::SOCK_BUFFER_RECV);
    Socket::setSendBufSize(s, Socket::SOCK_BUFFER_SEND);

    sockaddrin_t sai;
    MEMSET(&sai, 0x00, sizeof(sai));
    sai.sin_family = AF_INET;
    if (!ip || strlen(ip) == 0)
        sai.sin_addr.s_addr = INADDR_ANY;
    else
        sai.sin_addr.s_addr = inet_addr(ip);
    sai.sin_port = htons(port);

    if (TEMP_FAILURE_RETRY(::bind(s,
        reinterpret_cast<sockaddr_t*>(&sai), sizeof(sai))) < 0)
    {
        TEMP_FAILURE_RETRY(::close(s));
        return false;
    }

    if (TEMP_FAILURE_RETRY(::listen(s, backlog)) < 0)
    {
        TEMP_FAILURE_RETRY(::close(s));
        return false;
    }

    ns = s;
    return true;
}

bool TcpServerImpl::acceptImpl(socket_t s, sockaddrin_t* sa, socklen_t len, socket_t& ns)
{
    if (!s || s < 0 || !sa || !len)
        return false;

    _retry = false;
    int ret = -1;
    CLRERR();
    if ((ret = TEMP_FAILURE_RETRY(::accept(s, reinterpret_cast<sockaddr_t*>(sa), &len))) < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            _retry = true;
        return false;
    }

    ns = ret;
    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

