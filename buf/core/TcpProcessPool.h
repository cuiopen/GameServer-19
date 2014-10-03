
#ifndef TCPPROCESSPOOL_H_
#define TCPPROCESSPOOL_H_

#include "Event.h"
#include "TcpProcess.h"
#include "StateThread.h"
#include "ThreadGroup.h"

namespace buf
{

struct TPConfig
{
    int tmax; // Thread
    int pmax; // Processor
    const char* name;
};

class TcpProcessPool;

class StateThreadGroup : public ThreadGroup
{
public:
    StateThreadGroup(const char* name, const size_type maxsize)
        : ThreadGroup(name, maxsize) {}
    ~StateThreadGroup() {}

    bool add(const TcpProcessPool* pool, const TcpProcess* tp);
};

class TcpProcessPool
{
public:
    TcpProcessPool();
    virtual ~TcpProcessPool();

    virtual bool addAckEvent(EventManager& em, const TcpProcess* tp, const StateThread* st) = 0;
    virtual bool addProcessingEvent(EventManager& em, const TcpProcess* tp, const StateThread* st) = 0;

    virtual bool init(const TPConfig config[TPS_MAX]);

    bool add(const TcpProcess* tp);
    StateThread* get4Process(const TcpProcess* tp);
    void closeAll();
    void join();

private:
    StateThreadGroup* _tgs[TPS_MAX];
};

class AckThread : public StateThread
{
public:
    static const timediff_t TICK = 5;
    static const int PROCESSORS_PERTHREAD = 512;

public:
    AckThread(const char* name, TcpProcessState state, int maxprocessor)
        : StateThread(name, state, maxprocessor) {}
    virtual ~AckThread() {}

    virtual void changeState(const TcpProcess* tp);
    virtual void remove(const TcpProcess* tp)
    {
        DEBUG("%s: %d\n", __PRETTY_FUNCTION__, tp->fd());
        _em.delEvent(tp->fd());
        StateThread::remove(tp);
    }

    virtual void remove(iterator& i)
    {
        DEBUG("%s: %d\n", __PRETTY_FUNCTION__, (*i)->fd());
        _em.delEvent((*i)->fd());
        StateThread::remove(i);
    }

    virtual void run();

protected:
    virtual void _add(const TcpProcess* tp);

protected:
    EventManager _em;
};

class SyncThread : public StateThread
{
public:
    static const timediff_t TICK = 5;

public:
    SyncThread(const char* name, TcpProcessState state, int maxprocessor)
        : StateThread(name, state, maxprocessor) {}
    virtual ~SyncThread() {}

    virtual void run();
};

class ProcessingThread : public StateThread
{
public:
    static const timediff_t TICK = 5;
    static const int PROCESSORS_PERTHREAD = 512;

public:
    ProcessingThread(const char* name, TcpProcessState state, int maxprocessor)
        : StateThread(name, state, maxprocessor) {}
    virtual ~ProcessingThread() {}

    virtual void changeState(const TcpProcess* tp);
    virtual void run();

    virtual void remove(const TcpProcess* tp)
    {
        _em.delEvent(tp->fd());
        StateThread::remove(tp);
    }

    virtual void remove(iterator& i)
    {
        _em.delEvent((*i)->fd());
        StateThread::remove(i);
    }

protected:
    virtual void _add(const TcpProcess* tp);

protected:
    EventManager _em;
};

class RecycleThread : public StateThread
{
public:
    static const timediff_t TICK = 5;

public:
    RecycleThread(const char* name, TcpProcessState state, int maxprocessor)
        : StateThread(name, state, maxprocessor) {}
    virtual ~RecycleThread() {}

    virtual void run();
};

} // namespace buf

#endif // TCPPROCESSPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

