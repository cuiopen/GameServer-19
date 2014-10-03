#ifndef STATETHREAD_H_
#define STATETHREAD_H_

#include "Thread.h"
#include "ProcessQueue.h"
#include <list>

namespace buf
{

class StateThread : public ProcessQueue, public Thread
{
public:
    typedef std::list<TcpProcess*> container;
    typedef container::iterator iterator;

public:
    StateThread(const char* name, TcpProcessState state, int maxprocessor);
    virtual ~StateThread();

    inline TcpProcessState getState() const { return _state; }
    bool recycle(const TcpProcess* tp) { return tostate(tp, TPS_RECYCLE); }
    bool tostate(const TcpProcess* tp, TcpProcessState state = TPS_MAX);

    inline int size() const { return _size; }
    inline int rsize() const { return _maxprocessor - _size; }

    virtual void changeState(const TcpProcess*) {}

    virtual void _add(const TcpProcess* tp)
    {
        _tps.push_back(const_cast<TcpProcess*>(tp));
        ++_size;
        tp->setSwiching(false);
    }

    virtual void remove(const TcpProcess* tp)
    {
        //fprintf(stderr, "%s: %p\n", __PRETTY_FUNCTION__, tp);
        _tps.remove(const_cast<TcpProcess*>(tp));
        _size = _tps.size();
    }

    virtual void remove(iterator& i)
    {
        //fprintf(stderr, "%s: %p\n", __PRETTY_FUNCTION__, *i);
        i = _tps.erase(i);
        _size = _tps.size();
    }

    virtual void run() {}

protected:
    int _size;
    int _maxprocessor;

    TcpProcessState _state;
    container _tps;
};

} // namespace buf

#endif // STATETHREAD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

