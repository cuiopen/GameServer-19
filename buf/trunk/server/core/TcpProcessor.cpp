
#include "Buffer.h"
#include "TcpProcessor.h"
#include "StateThread.h"

namespace buf
{

int TcpProcessor::sendPack(const void* buf, int size)
{
    return _sock.sendPack(buf, size, isCached());
}

int TcpProcessor::recvPack()
{
    // XXX: when server is slower than client there will be more data in recv buffer
    int ret = 0;
    do
    {
        // XXX: if peer is closed it will give up the data in buffer
        if (_sock.isPeerClosed())
        {
            setState(TPS_RECYCLE);
            return 0; // XXX: hack return 0 not -1
        }

        //Buffer buffer;
        StackBuffer<RECV_BUF_SIZE> buffer;
        int len = _sock.recvPack(buffer.wbuf(), buffer.wsize());
        if (len < 0)
        {
            setState(TPS_RECYCLE);
            return 0; // XXX: hack return 0 not -1
        }

        buffer.wflip(len);

        switch (_state)
        {
            case TPS_ACK:
                ret = ack(buffer.rbuf(), buffer.rsize());
                break;

            case TPS_SYNC:
                break;

            case TPS_PROCESSING:
                ret = process(buffer.rbuf(), buffer.rsize());
                break;

            case TPS_RECYCLE:
                // XXX: IMPOSSIABLE - do nothing
                break;

            case TPS_MAX:
            default:
                // XXX: IMPOSSIABLE
                setState(TPS_RECYCLE);
                break;
        }

        switch (ret)
        {
            case 0:
                break;

            case 1:
                nextState();
                break;

            case -1:
                setState(TPS_RECYCLE);
                break;

            default:
                break;
        }
    } while (_sock.isBuffered());

    return ret;
}

void TcpProcessorAckEvent::onError()
{
}

void TcpProcessorAckEvent::onEof()
{
    TcpProcessor* tp = reinterpret_cast<TcpProcessor*>(this->tp());
    if (!tp)
        return;
    tp->setState(TPS_RECYCLE);
    if (_st)
        _st->changeState(tp);
}

bool TcpProcessorAckEvent::onEvent(event_t ev)
{
    TcpProcessor* tp = reinterpret_cast<TcpProcessor*>(this->tp());
    if (!tp)
        return false;

    bool ret = true;
    if (ev & EVENT_READ)
    {
        if (tp->recvPack() < 0)
        {
            tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_WRITE)
    {
        if (tp->isCached() && !tp->syncs())
        {
            tp->onDisconnected();
            tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_ERROR)
    {
        tp->onDisconnected();
        tp->setState(TPS_RECYCLE);
        ret = false;
    }

    if (_st)
        _st->changeState(tp);

    return ret;
}

void TcpProcessorProcessingEvent::onError()
{
}

void TcpProcessorProcessingEvent::onEof()
{
    TcpProcessor* tp = reinterpret_cast<TcpProcessor*>(this->tp());
    if (!tp)
        return;
    tp->setState(TPS_RECYCLE);
    if (_st)
        _st->changeState(tp);
}

bool TcpProcessorProcessingEvent::onEvent(event_t ev)
{
    TcpProcessor* tp = reinterpret_cast<TcpProcessor*>(this->tp());
    if (!tp)
        return false;

#if 0
    if (tp->getState() != _st->getState()) // XXX: ignor
        return true;
#endif

    bool ret = true;
    if (ev & EVENT_READ)
    {
        if (tp->recvPack() < 0)
        {
            tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_WRITE)
    {
        if (tp->isCached() && !tp->syncs())
        {
            tp->onDisconnected();
            tp->setState(TPS_RECYCLE);
            ret = false;
        }
    }

    if (ev & EVENT_ERROR)
    {
        tp->onDisconnected();
        tp->setState(TPS_RECYCLE);
        ret = false;
    }

    if (_st)
        _st->changeState(tp);

    return ret;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

