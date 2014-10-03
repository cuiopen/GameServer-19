
#include "Socket.h"

namespace buf
{

Socket::Socket(socket_t s, const sockaddrin_t* sa)
    : SocketImpl(s, sa), _first(true), _buffered(false), _zipon(true),
    _encon(false), _enctype(0), _recv(SOCK_BUFFER_RECV), _send(SOCK_BUFFER_SEND)
{
    setNoDelay();
    setNonblock();
    setRecvBufSize(SOCK_BUFFER_RECV);
    setSendBufSize(SOCK_BUFFER_SEND);
}

int Socket::recvPack(void* buf, int size, bool poll)
{
    int ret = 0;

    if (_recv.rsize())
    {
        PackHead_t& h = PH_HEAD(_recv.rbuf());
        if (PH_LEN(h) + PH_SIZE() <= _recv.rsize())
        { // already in _recv
            return popPack(buf, size, _recv);
        }
    }

    int pkgsize = 0;
    do
    {
        if (poll)
        {
            ret = syncRecvImpl(_recv.wbuf(), _recv.wsize());
        }
        else
        {
            ret = recvImpl(_recv.wbuf(), _recv.wsize());
        }

        if (!ret)
            continue;

        if (ret < 0)
        {
            _recv.clear();
            return -1;
        }

        // XXX: <policy></policy>
        if (_first && ret >= 4)
        {
            char* pbuf = (char*)_recv.rbuf();
            if(pbuf[2] == 'o' && pbuf[0] == '<' && pbuf[1] == 'p' && pbuf[3] == 'l')
            {
                // TODO: put this into a configuration
                char resp[128] = {0};
                int len = sprintf(resp, "<?xml version=\"1.0\"?>"
                        "<cross-domain-policy>"
                        "<allow-access-from domain=\"*\" to-ports=\"*\" />"
                        "</cross-domain-policy>");

                sendImpl(resp, len);
                _first = false;
                return -1;
            }
        }

        _recv.wflip(ret);

        if (_first && static_cast<unsigned int>(ret) >= PH_SIZE())
        {
            _first = false;

            PackHead_t& h = PH_HEAD(_recv.rbuf());
            pkgsize = static_cast<int>(PH_LEN(h));
            if (!pkgsize) // empty packet
                break;
            if (pkgsize+PH_SIZE() > SOCK_BUF_MAX_SIZE)
            {
                fprintf(stderr, "%s: pkgsize(%lu) > (SOCK_BUF_MAX_SIZE)%u\n",
                        __PRETTY_FUNCTION__, pkgsize + PH_SIZE(), SOCK_BUF_MAX_SIZE);
                _recv.clear();
                return -1;
            }
            if (ret == pkgsize + static_cast<int>(PH_SIZE())) // no more data
                break;
        }

        if (_recv.rsize() >= pkgsize + PH_SIZE()) // reach to the end of one packet
            break;
        if (!_recv.wsize() && (_recv.rsize() < pkgsize + PH_SIZE()))
        {
            // XXX: no more memory, the packet size larger than SOCK_BUFFER
            _recv.clear();
            return -1; // XXX: may be hacker
        }
    } while (true);
    return popPack(buf, size, _recv);
}

// XXX: only copy size bytes to buf of one pack.
//      So you must supply enought buffer to recive data.
//      Default is 64K.
int Socket::popPack(void* buf, int size, Buffer& buffer)
{
    PackHead_t& h = PH_HEAD(buffer.rbuf());
    int nsize = size >= PH_LEN(h) ? PH_LEN(h) : size;
    //if (PH_ENCRYPTED(h))
    //{
    //}
    if (PH_ZIPFLAG(h))
    {
        _unzipper.unzipper(buffer.rbuf()+PH_SIZE(), PH_LEN(h));
        nsize = size >= static_cast<int>(_unzipper.size()) ? _unzipper.size() : size;
        MEMCPY(buf, &_unzipper, nsize);
    }
    else
    {
        nsize = size >= PH_LEN(h) ? PH_LEN(h) : size;
        MEMCPY(buf, buffer.rbuf()+PH_SIZE(), nsize);
    }

    buffer.rflip(PH_SIZE()+PH_LEN(h)); // XXX: next pack if have
    if (buffer.rsize())
        _buffered = true;
    else
        _buffered = false;

    return nsize;
}

void Socket::pushPack(const void* buf, int size, Buffer& buffer)
{
    PackHead_t h;
    if (_zipon && size >= static_cast<int>(SOCK_MINZIP_SIZE))
    {
        _zipper.zipper(buf, size);
        PH_ZIPFLAG(h) = 1;
        if (isEncryptOn())
        { // TODO:
            // PH_ENCTYPE(h) = _enctype;
            // Encypter enc(&_zipper, _zipper.size(), PH_ENCTYPE(h));
        }

        PH_LEN(h) = _zipper.size();
        buffer << PH(h);
        buffer.put(reinterpret_cast<Buffer::const_pointer>(&_zipper), _zipper.size()); 
        return;
    }

    if (isEncryptOn())
    { // TODO:
        PH_ENCTYPE(h) = _enctype;
        buffer << PH(h);
        // size_t off = buffer.curr();
        buffer.put(reinterpret_cast<Buffer::const_pointer>(buf), size);
        // Encypter enc(&buffer[off], size, PH_ENCTYPE(h));
        return;
    }

    PH_LEN(h) = size;
    buffer << PH(h);
    buffer.put(reinterpret_cast<Buffer::const_pointer>(buf), size);
    return;
}

int Socket::sendPack(const void* buf, int size, bool cache)
{
    if (!buf || !size)
        return 0;

    if (cache)
    {
        pushPack(buf, size, _send);
        return size;
    }

    Buffer snd;
    pushPack(buf, size, snd);
    return syncSendImpl(snd.rbuf(), snd.rsize());
}

bool Socket::syncs()
{
    if (_send.rsize())
    {
        int ret = sendImpl(_send.rbuf(), _send.rsize());
        if (ret > 0)
            _send.rflip(ret);
        if (ret < 0)
            return false;
    }
    return true;
}

void Socket::forceSync()
{
    if (_send.rsize())
    {
        int ret = syncSendImpl(_send.rbuf(), _send.rsize());
        if (ret > 0)
            _send.rflip(ret);
        if (ret < 0)
            return;
        if (_send.rsize())
        {
            (void)0; // XXX: warning
        }
    }
}

void Socket::close()
{
    forceSync();
    closeImpl();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

