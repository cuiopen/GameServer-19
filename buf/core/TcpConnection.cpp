
#include "TcpConnection.h"
#include "Buffer.h"
#include "Thread.h"
#include "Logger.h"

namespace buf
{

bool TcpConnection::connect(const char* sn, bool reconnect)
{
    setReconnect(reconnect);
    do
    {
        sLog.trace("Connecting to %s: %s:%d.", sn?sn:"Server", getIp(), getPort());
        if (!connect(_ip, _port))
        {
            sLog.append(" [FAILED]\n");
            Thread::sleep(1);
        }
        else
        {
            sLog.append(" [SUCC]\n");
        }
    } while(!connected() && isReconnect());
    return true;
}

bool TcpConnection::connect(const char* ip, port_t port)
{
    if (!port)
        return false;
    if (!ip || strlen(ip) == 0 || strncmp(ip, "nil", 3) == 0)
        ip = "127.0.0.1";
    cpystr(ip, _ip, sizeof(_ip));
    _port = port;
    _sock = connectImpl(ip, port);
    if (_sock != 0)
    {
        onConnected();
        return true;
    }
    return false;
}

int TcpConnection::send(const void* buf, int size)
{
    if (_sock)
        return _sock->send(buf, size);
    return 0;
}

int TcpConnection::recv(void* buf, int size)
{
    if (_sock)
        return _sock->recv(buf, size);
    return 0;
}

int TcpConnection::syncRecv(void* buf, int size)
{
    if (_sock)
        return _sock->syncRecv(buf, size);
    return 0;
}

// XXX: 如果isCached()==true发送时只放在发送缓存中，通过OUT事件调用syncs()
int TcpConnection::sendPack(const void* buf, int size)
{
    if (_sock)
        return _sock->sendPack(buf, size, isCached());
    return 0;
}

int TcpConnection::forceSendPack(const void* buf, int size)
{
    if (_sock)
        return _sock->sendPack(buf, size);
    return 0;
}

int TcpConnection::recvPack()
{
    if (!_sock)
        return -1;

    int ret = 0;
    do
    {
        StackBuffer<RECV_BUF_SIZE> buffer;
        int len = _sock->recvPack(buffer.wbuf(), buffer.wsize());
        if (len < 0)
        {
            setState(TPS_RECYCLE);
            return -1;
        }

        buffer.wflip(len);

        switch (_state)
        {
            case TPS_ACK:
                // XXX: !!!WARNING!!! We have no SyncThread
                ret = ack(reinterpret_cast<const void*>(buffer.rbuf()), buffer.rsize());
                if (ret == 1)
                    setState(TPS_PROCESSING);
                return ret;
                break;

            case TPS_PROCESSING:
                ret = process(reinterpret_cast<const void*>(buffer.rbuf()), buffer.rsize());
                break;

                // XXX: IMPOSSIABLE - do nothing
            case TPS_SYNC:
                // XXX: IMPOSSIABLE - do nothing
            case TPS_RECYCLE:
                // XXX: IMPOSSIABLE - do nothing
            case TPS_MAX:
            default:
                // XXX: IMPOSSIABLE
                setState(TPS_RECYCLE);
                return -1;
                break;
        }
    } while (_sock->isBuffered());

    return ret;
}

int TcpConnection::recvPack(void* buf, int size)
{
    UNUSE(buf);
    UNUSE(size);

    int ret = 0;
    return ret;
}

void TcpConnectionAckEvent::onError()
{
}

void TcpConnectionAckEvent::onEof()
{
    static_cast<TcpConnection*>(tp())->recycle();
}

bool TcpConnectionAckEvent::onEvent(event_t ev)
{
    TcpConnection* _tp = static_cast<TcpConnection*>(tp());
    if (!_tp)
        return false;

    bool ret = true;
    if (ev & EVENT_READ)
    {
        if (_tp->recvPack() < 0)
        {
            _tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_WRITE)
    {
        if (_tp->isCached() && !_tp->syncs())
        {
            _tp->onDisconnected();
            _tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_ERROR)
    {
        _tp->onDisconnected();
        _tp->setState(TPS_RECYCLE);
        ret = false;
    }

    if (_st)
        _st->changeState(tp());

    return true;
}

void TcpConnectionProcessingEvent::onError()
{
}

void TcpConnectionProcessingEvent::onEof()
{
    static_cast<TcpConnection*>(tp())->recycle();
}

bool TcpConnectionProcessingEvent::onEvent(event_t ev)
{
    TcpConnection* _tp = static_cast<TcpConnection*>(tp());
    if (!_tp)
        return false;

    bool ret = true;
    if (ev & EVENT_READ)
    {
        if (_tp->recvPack() < 0)
        {
            _tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_WRITE)
    {
        if (_tp->isCached() && !_tp->syncs())
        {
            _tp->onDisconnected();
            _tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_ERROR)
    {
        _tp->onDisconnected();
        _tp->setState(TPS_RECYCLE);
        ret = false;
    }

    if (_st)
        _st->changeState(_tp);

    return ret;
}

}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

