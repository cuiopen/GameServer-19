
#ifndef SOCKET_H_
#define SOCKET_H_

#include "Config.h"
#include "Noncopyable.h"
#ifdef UNIXLIKE
#include "linux/Socket_POSIX.h"
#endif
#include "Zipper.h"
#include "Unzipper.h"
#include "Buffer.h"
#include "Mutex.h"

namespace buf
{

#define HOST_ANY "0.0.0.0"
#define PORT_ANY 8000

typedef struct PackHead
{
    PackHead() { h.header = 0; }

    union Head
    {
        struct Header
        {
            unsigned int length : 28;
            unsigned int enctype : 3; // 0-not encrypted 1-7
            unsigned int zipped : 1;
        } h;
        unsigned int header;
    } h;
} PackHead_t;

#define PH_LEN(x)       (x).h.h.length
#define PH_ENCTYPE(x)   (x).h.h.enctype
#define PH_ENCRYPTED(x) (x).h.h.enctype
#define PH_ZIPFLAG(x)   (x).h.h.zipped
#define PH(x) (x).h.header
#define PH_SIZE() sizeof(PackHead_t)
#define PH_HEAD(x) *((PackHead_t*)(x));

class Socket : public SocketImpl, public Noncopyable
{
public:
    static const size_t SOCK_BUFFER_RECV = SOCKIMPL_BUFFER_RECV;
    static const size_t SOCK_BUFFER_SEND = SOCKIMPL_BUFFER_SEND;

    static const int    SOCK_RD_TIMEOUT = SOCKIMPL_RD_TIMEOUT;    // msecs
    static const int    SOCK_WR_TIMEOUT = SOCKIMPL_WR_TIMEOUT;    // msesc

    static const size_t SOCK_UNZIP_BUFFER = ((SOCK_BUFFER_RECV * 150) / 100) + 128;
    static const size_t SOCK_MINZIP_SIZE = 32; // XXX: minimal zip size

    static const int SOCK_ENC_NONE  = 0x0;
    static const int SOCK_ENC_DES   = 0x1;
    static const int SOCK_ENC_RC5   = 0x2;
    static const int SOCK_ENC_X1    = 0x3;
    static const int SOCK_ENC_X2    = 0x4;
    static const int SOCK_ENC_X3    = 0x5;
    static const int SOCK_ENC_X4    = 0x6;
    static const int SOCK_ENC_X5    = 0x7;

public:
    inline static void setSendBufSize(socket_t s, int size) { SocketImpl::setSendBufSizeImpl(s, size); }
    inline static void setRecvBufSize(socket_t s, int size) { SocketImpl::setRecvBufSizeImpl(s, size); }
    inline static void setReuse(socket_t s, bool reuse) { SocketImpl::setReuseImpl(s, reuse); }
    inline static void setNoDelay(socket_t s, bool delay) { SocketImpl::setNoDelayImpl(s, delay); }
    inline static void setNonblock(socket_t s, bool nonblock) { SocketImpl::setNonblockImpl(s, nonblock); }

    inline static const char* getIpByIf(const char* inet) { return getIpByIfImpl(inet); }
    inline static int addr2net(const char* ip) { return addr2netImpl(ip); }
    inline static const char* net2addr(int ip) { return net2addrImpl(ip); }
    inline static u32_t toNetLong(u32_t hl) { return toNetLongImpl(hl); }
    inline static u16_t toNetShort(u16_t hs) { return toNetShortImpl(hs); }
    inline static u32_t toHostLong(u32_t nl) { return toHostLongImpl(nl); }
    inline static u16_t toHostShort(u16_t ns) { return toHostShortImpl(ns); }

public:
    explicit Socket(socket_t s, const sockaddrin_t* sa);
    ~Socket()
    {
        close();
    }

    inline socket_t fd() const { return fdImpl(); }

    inline void setZip(bool onoff = true) { _zipon = onoff; }
    inline void setZipOn() { setZip(true); }
    inline void setZipOff() { setZip(false); }
    inline bool isZipOn() const { return _zipon; }

    inline void setEncrypt(bool onoff = true) { _encon = onoff; }
    inline void setEncryptOn() { setEncrypt(true); }
    inline void setEncryptOff() { setEncrypt(false); }
    inline bool isEncryptOn() const { return _encon; }
    inline int getEnctyptType() const { return _enctype; }
    inline void setEncrypt(int type)
    {
        _enctype = type;
        if (type == SOCK_ENC_NONE) {
            setEncrypt(false);
            return;
        }
        setEncrypt(true);
    }

    // >= 0 - try again
    // < 0  - ERROR shoud close

    // origin data recv
    inline int recv(void* buf, int size) { return recvImpl(buf, size); }
    // origin data send XXX: should resend when return value less than size
    int send(const void* buf, int size) { return sendImpl(buf, size); }

    // origin recv XXX: must recived
    inline int syncRecv(void* buf, int size) { return syncRecvImpl(buf, size); }
    // origin data sync send XXX: all date must be sent
    inline int syncSend(const void* buf, int size) { return syncSendImpl(buf, size); }

    // packet recv XXX: all data must be recved
    int recvPack(void* buf, int size, bool poll = false);
    inline int recvPackWithPoll(void* buf, int size) { return recvPack(buf, size, true); }
    // packet send XXX: all data must be sent with poll
    int sendPack(const void* buf, int size, bool cache = false);

    inline bool incompleter() const { return incompleterImpl(); }
    inline bool incompletew() const { return incompletewImpl(); }

    inline bool isBuffered() const { return _buffered; }

    inline bool closed() const { return closedImpl(); }
    inline bool connected() const { return connectedImpl(); }
    inline bool disconnected() const { return disconnectedImpl(); }

    inline const char* getLocalIp() const { return getLocalIpImpl(); }
    inline unsigned short getLocalPort() const { return getLocalPortImpl(); }
    inline const char* getRemoteIp() const { return getRemoteIpImpl(); }
    inline int getRemoteIpRaw() const { return getRemoteIpRawImpl(); }
    inline unsigned short getRemotePort() const { return getRemotePortImpl(); }

    inline void setSendBufSize(int size) { setSendBufSize(fd(), size); }
    inline void setRecvBufSize(int size) { setRecvBufSize(fd(), size); }
    inline void setReuse(bool reuse) { setReuse(fd(), reuse); }
    inline void setNoDelay() { setNoDelay(fd(), true); }
    inline void setDelay() { setNoDelay(fd(), false); }
    inline void setNonblock() { setNonblock(fd(), true); }
    inline void setBlock() { setNonblock(fd(), false); }

    bool syncs();
    void forceSync();
    void close();

private:
    void pushPack(const void* buf, int size, Buffer& buffer);
    int popPack(void* buf, int size, Buffer& buffer);

private:
    bool _first;
    bool _buffered;
    bool _zipon;
    bool _encon;
    int _enctype;
    Buffer _recv; // buffer for unzipping or decrypting
    Buffer _send; // buffer for zipping or encrypting
    Zipper _zipper;
    Unzipper _unzipper;
};

} // namespace buf

#endif // SOCKET_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

