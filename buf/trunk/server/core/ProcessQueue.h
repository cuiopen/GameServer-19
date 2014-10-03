
#ifndef PROCESSQUEUE_H_
#define PROCESSQUEUE_H_

#include "Config.h"
#include "Mutex.h"
#include <queue>

namespace buf
{

class ProcessQueue
{
public:
    typedef std::queue<TcpProcess*> queue;

public:
    ProcessQueue() : _queuesize(0) {}
    virtual ~ProcessQueue() {}

    void add(const TcpProcess* tp)
    {
        ScopeLock lock(_lock);
        _queue.push(const_cast<TcpProcess*>(tp));
        ++_queuesize;
    }

    void checkQueue()
    {
        if (!_queuesize)
            return;

        {
            ScopeLock lock(_lock);
            while(!_queue.empty())
            {
                _tmpqueue.push(_queue.front());
                _queue.pop();
            }
            _queuesize = 0;
        }

        while (!_tmpqueue.empty())
        {
            TcpProcess* tp = _tmpqueue.front();
            _tmpqueue.pop();
            _add(tp);
        }
    }

    virtual void _add(const TcpProcess* tp) = 0;

private:
    Mutex _lock;
    int _queuesize;
    queue _queue;
    queue _tmpqueue;
};

} // namespace buf

#endif // PROCESSQUEUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

