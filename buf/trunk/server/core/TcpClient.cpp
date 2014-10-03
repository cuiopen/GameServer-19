
#include "TcpClient.h"
#include "Buffer.h"
#include "Event.h"

namespace buf
{

void TcpClient::run()
{
    if (!connected())
        return;

    EventManager em;
    if (!em.addEvent(BUFNEW TcpClientEvent(fd(), EVENT_ALL, this)))
        return;

    while (!isFinal())
    {
        if (em.wait(0) < 0)
        {
            em.exit();
            final();
        }
        Thread::msleep(TICK);

        if (disconnected())
        {
            em.exit();
            final();
        }
    }
    onFinal();
}

void TcpClientEvent::onError()
{
}

void TcpClientEvent::onEof()
{
    if (_tc)
    {
        _tc->onDisconnected();
        _tc->close();
        _tc->final();
    }
}

bool TcpClientEvent::onEvent(event_t ev)
{
    if (!_tc)
        return false;

    if (ev & EVENT_READ)
    {
        if (_tc->recvPack() < 0)
        {
            _tc->onDisconnected();
            _tc->close();
            _tc->final();
            return false;
        }
    }

    if (ev & EVENT_WRITE)
    {
        if (_tc->isCached() && !_tc->syncs())
        {
            _tc->onDisconnected();
            _tc->close();
            _tc->final();
            return false;
        }
    }

    if (ev & EVENT_ERROR)
    {
        _tc->onDisconnected();
        _tc->close();
        _tc->final();
    }
    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

