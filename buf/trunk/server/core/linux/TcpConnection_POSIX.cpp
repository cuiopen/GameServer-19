
#include "TcpConnection_POSIX.h"
#include <unistd.h> // for TEMP_FAILURE_RETRY
#include <errno.h> // for errno

namespace buf
{

Socket* TcpConnectionImpl::connectImpl(const char* ip, port_t port)
{
    if (!ip || !port)
        return 0;

    socket_t s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
        return 0;

    sockaddrin_t sai;
    MEMSET(&sai, 0x00, sizeof(sai));
    sai.sin_family = AF_INET;
    sai.sin_addr.s_addr = inet_addr(ip);
    sai.sin_port = htons(port);

    // XXX:
    Socket::setNonblock(s, true);

    int ret = TEMP_FAILURE_RETRY(::connect(s,
                reinterpret_cast<sockaddr_t*>(&sai), sizeof(sai)));
    if (ret < 0)
    {
        struct timeval tv;
        fd_set set;
        FD_ZERO(&set);
        FD_SET(s, &set);

        tv.tv_sec = 3;
        tv.tv_usec = 0;

        if (select(s+1, &set, &set, NULL, &tv) > 0) 
        {
            int error = -1;
            int len = sizeof(error);
            getsockopt(s, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if (error == 0)
                goto _succ;
        }

        TEMP_FAILURE_RETRY(::close(s));
        return 0;
    }

_succ:
    Socket* sock = BUFNEW Socket(s, &sai);
    if (!sock)
    {
        TEMP_FAILURE_RETRY(::close(s));
        return 0;
    }
    return sock;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

