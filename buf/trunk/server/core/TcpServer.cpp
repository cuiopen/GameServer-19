
#include "TcpServer.h"
#include "Event.h"
#include "Logger.h"

namespace buf
{

TcpServer::TcpServer(const char* name, int maxprocessor)
    : Thread(name), _sock(-1), _maxprocessor(maxprocessor),
    _bound(false), _extport(0) {}

TcpServer::~TcpServer()
{
    if (_sock > 0)
    {
        TEMP_FAILURE_RETRY(::shutdown(_sock, SHUT_RDWR));
        TEMP_FAILURE_RETRY(::close(_sock));
        _sock = -1;
    }
}

bool TcpServer::init(const TPConfig* cfg)
{
    if (!_pool.init(cfg))
        return false;
    return true;
}

TcpProcessor* TcpServer::accept()
{
    socket_t s;
    sockaddrin_t sa;
    TcpProcessor* tp = 0;
    if (acceptImpl(_sock, &sa, sizeof(sa), s))
    {
        tp = createProcessor(s, &sa);
        if (tp)
            tp->onConnected();
        if (tp && _pool.add(tp))
            return tp;

        tp->onDisconnected();
        DELETENULL(tp);
    }
    return 0;
}

class AcceptEvent : public Event
{
public:
    AcceptEvent(socket_t s, event_t evs, TcpServer* ts)
        : Event(s, evs), _ts(ts) {}

    virtual bool onEvent(event_t ev)
    {
        UNUSE(ev);
        if (_ts)
        {
            TcpProcessor* tp = _ts->accept();
            return tp == 0;
        }
        return false;
    }

private:
    TcpServer* _ts;
};

void TcpServer::run()
{
    EventManager em(TCPS_QUEUE);
    sLog.trace("%s: Add AcceptEvent with sd (%d)\n", __PRETTY_FUNCTION__, _sock);
    if (!em.addEvent(BUFNEW AcceptEvent(_sock, EVENT_READ, this)))
    {
        sLog.error("%s: Register AcceptEvent(%d)\n", __PRETTY_FUNCTION__, _sock);
        //em.exit();
        //final();
    }

    RTime rt;
    while (!isFinal())
    {
        if (em.wait(0) < 0)
        { // XXX: SYSTEM ERROR
            sLog.error("%s: wait() < 0 with errno: %d\n", __PRETTY_FUNCTION__, errno);
            em.exit();
            final();
        }

        if (rt.elapse() < TICK)
            Thread::msleep(TICK);
        rt.now();
    }
    _pool.join();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

