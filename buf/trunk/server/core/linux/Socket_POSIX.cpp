
#include "Config.h"
#include "Socket_POSIX.h"
#include <string.h> // for strncpy
#include <sys/ioctl.h> // for ioctl
#include <net/if.h> // for struct ifreq, IF_NAMESIZE
#include <poll.h> // for poll
#include <unistd.h> // for TEMP_FAILURE_RETRY
#include <errno.h> // for errno, EINTR

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

namespace buf
{

SocketImpl::SocketImpl(socket_t s, const sockaddrin_t* sa)
    : _sock(s), _incompleter(false), _incompletew(false), _peerclosed(false)
{
    if (sa)
    {
        _rsa = *sa;
    }
    else
    {
        socklen_t len = sizeof(_rsa);
        getpeername(_sock, reinterpret_cast<sockaddr_t*>(&_rsa), &len);
    }

    bzero(&_lsa, sizeof(_lsa));
    socklen_t len = sizeof(_lsa);
    getsockname(_sock, reinterpret_cast<sockaddr_t*>(&_rsa), &len);

    // XXX: aviable in child but not in father
    int flag = fcntl(_sock, F_GETFD, 0);
    flag |= FD_CLOEXEC;
    fcntl(_sock, F_SETFD, flag);
}

SocketImpl::~SocketImpl()
{
    closeImpl();
}

const char* SocketImpl::getIpByIfImpl(const char* inet)
{
    if (!inet)
        return 0;

    int fd;
    fd = socket(AF_INET , SOCK_DGRAM , 0);
    if (fd < 0)
        return 0;

    struct ifreq ifr;
    size_t ilen = strlen(inet);
    ilen = ilen > IF_NAMESIZE ? IF_NAMESIZE - 1: ilen;
    MEMZRO(ifr.ifr_name, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name , inet, ilen);
    if (ioctl(fd, SIOCGIFADDR , &ifr) < 0)
        return 0;

    return inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr);
}

int SocketImpl::recvImpl(void* buf, int size)
{
    if (!buf || !size || _sock < 0 || _peerclosed)
        return -1;
    
    CLRERR();

    // XXX: try again when return -1 and errno == EINTR
    int ret = TEMP_FAILURE_RETRY(::recv(_sock, buf, size, MSG_NOSIGNAL));
    if (ret < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            _incompleter = true;
            return 0; // should be try again
        }
        else
        {
            DEBUG("errno: %d\n", errno);
            return -1; // ERROR
        }
    }
    if (!ret)
    { // peer is closed when ret is 0
        _peerclosed = true;
        return -1;
    }
    return ret;
}

int SocketImpl::syncRecvImpl(void* buf, int size)
{
    if (!buf || !size || _sock < 0 || _peerclosed)
        return -1;

    CLRERR();

    int ret = 0;

    if (_incompleter)
    {
        _incompleter = false;
        goto _poll1;
    }

    do
    {
        // XXX: try again when return -1 and errno == EINTR
        ret = TEMP_FAILURE_RETRY(::recv(_sock, buf, size, MSG_NOSIGNAL));
        if (!ret)
        {
            DEBUG("peerclosed\n");
            _peerclosed = true;
            return -1; // peer is closed when ret is 0
        }
        if (ret < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                _incompleter = true;
_poll1:
                if (wait4read(_sock) < 0)
                {
                    DEBUG("errno: %d\n", errno);
                    return -1; // ERROR
                }
            }
        }
    } while (ret < 0);
    _incompleter = false;
    return ret;
}

int SocketImpl::sendImpl(const void* buf, int size)
{
    if (!buf || !size || _sock < 0 || _peerclosed)
        return -1;

    CLRERR();

    int off = 0;
    int ret = 0;
    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(buf);
    do
    {
        // XXX: try again when return -1 and errno == EINTR
        ret = TEMP_FAILURE_RETRY(::send(_sock, buffer+off, size-off, MSG_NOSIGNAL));
        if (ret < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                _incompletew = true;
                return off; // should be try again
            }
            else
            {
                DEBUG("errno: %d\n", errno);
                return -1; // ERROR
            }
        }
        off += ret;
    } while (off < size);
    return off;
}

int SocketImpl::syncSendImpl(const void* buf, int size)
{
    if (!buf || !size || _sock < 0 || _peerclosed)
        return -1;

    if (_peerclosed)
        return -1;

    CLRERR();

    int off = 0;
    int ret = 0;
    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(buf);

    if (_incompletew)
    {
        _incompletew = false;
        goto _poll2;
    }

    do
    {
        // XXX: try again when return -1 and errno == EINTR
        ret = TEMP_FAILURE_RETRY(::send(_sock, buffer+off, size-off, MSG_NOSIGNAL));
        if (ret < 0)
        {
            ret = 0;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                _incompletew = true;
_poll2:
                if (wait4send(_sock) < 0) // continue when return 0
                    return -1;
            }
            else
            {
                DEBUG("errno: %d\n", errno);
                return -1; // ERROR, should be terminated
            }
        }
        off += ret;
    } while (off < size);
    _incompletew = false;
    return off;
}

// >0 - success
// 0 - timeout
// -1 - error
int SocketImpl::wait4send(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLOUT | POLLERR | POLLPRI;
    pfd.revents = 0;

    int ret = TEMP_FAILURE_RETRY(::poll(&pfd, 1, SOCKIMPL_WR_TIMEOUT));
    if (ret > 0 && !(pfd.revents & POLLOUT))
        return -1;

    return ret;
}

int SocketImpl::wait4read(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLERR | POLLPRI;
    pfd.revents = 0;

    int ret = TEMP_FAILURE_RETRY(::poll(&pfd, 1, SOCKIMPL_RD_TIMEOUT));
    if (ret > 0 && !(pfd.revents & POLLIN))
        return -1;

    return ret;
}

void SocketImpl::closeImpl()
{
    if (_sock > 0)
    {
        TEMP_FAILURE_RETRY(::shutdown(_sock, SHUT_RDWR));
        TEMP_FAILURE_RETRY(::close(_sock));
    }
    _sock = -1;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

