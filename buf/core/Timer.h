
#ifndef TIMER_H_
#define TIMER_H_

#include "Config.h"
#include "Noncopyable.h"
#include "Thread.h"
#include "Mutex.h"
#include "RTime.h"
#include "MinHeap.h"

#if defined(USE_EPOLL_TIMER) && USE_EPOLL_TIMER
#include "linux/Timer_EPOLL.h"
#elif defined(USE_KQUEUE) && USE_KQUEUE && __APPLE__
#include "mac/Timer_KQUEUE.h"
#else
#error "no implementation"
#endif

namespace buf
{
 
class Timer
{
    friend class TimerManager;
public:
    static const unsigned int UNLIMITED = -1;

    static const short STOPPED = 0x01;
    static const short PAUSED  = 0x02;
    static const short RUNNING = 0x03;
    static const short DISABLED= 0x04;

public:
    Timer(u32_t interval, u32_t count = UNLIMITED)
        : _interval(interval), _count(count), _state(STOPPED), _timeout(interval) {}
    virtual ~Timer()
    {
#ifndef _NDEBUG
        fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
#endif
    }

    // XXX: onTimer should be returned in short time
    virtual bool onTimer()
    {
#ifndef _NDEBUG
        fprintf(stderr, "%s: interval: %u, count: %u\n", __PRETTY_FUNCTION__, _interval, _count);
#endif

        return false;
    }

    inline bool isPaused() const { return _state == PAUSED; }
    inline bool isStopped() const { return _state == STOPPED; }
    inline bool isRunning() const { return _state == RUNNING; }
    inline bool isDisabled() const { return _state == DISABLED; }

    inline u32_t getInterval() const { return _interval; }
    inline void setInterval(unsigned int interval) { _interval = interval; }

    inline u32_t getCount() const { return _count; }
    inline void setCount(unsigned int count) { _count = count; }
    inline void decCount() { if (_count && _count != UNLIMITED) --_count; }
    inline void incCount() { ++_count; }

    inline const RTime& getTimeout() const { return _timeout; }
    inline void setTimeout(const RTime& timeout) { _timeout = timeout; }

    inline bool operator>(const Timer& t) const { return this->_timeout > t._timeout; }

private:
    inline void* getOwner() const { return _owner; }
    inline void* setOwner(void* owner) { _owner = owner; return _owner; }

protected:
    u32_t _interval;
    u32_t _count;
    short _state;
    RTime _timeout;

private:
    void* _owner; //owner node in MinHeap
};

struct TimerGreater
{
    inline bool operator()(const Timer& t1, const Timer& t2) const
    {
        return t1 > t2;
    }
    inline bool operator()(const Timer* t1, const Timer* t2) const
    {
        return *t1 > *t2;
    }
};

class TimerManager : public TimerImpl, public Noncopyable
{
public:
    typedef MinHeap<Timer*, TimerGreater> TimerMinHeap_t;

public:
    static const int IDL_EMPTY_TIME = 5; // micro seconds

public:
    TimerManager() : _terminated(false) {}
    ~TimerManager() { clear(); }

    bool addTimer(Timer* timer);
    bool delTimer(Timer* timer);
    bool stopTimer(Timer* timer);
    bool startTimer(Timer* timer);
    bool pauseTimer(Timer* timer);
    bool continueTimer(Timer* timer);
    int wait();
    void terminated();

    void clear();

protected:
    void correct_time(unsigned int usecs);
    void signal(Timer* timer);

private:
    Mutex _lock;
    TimerMinHeap_t _heap;
    bool _terminated;
};

class TimerThread : public TimerManager, public Thread
{
public:
    TimerThread() : Thread("TimerThread") {}
    TimerThread(const char* name) : Thread(name) {}
    virtual ~TimerThread() {}
    virtual void run();
};

} // namespace buf

#endif // TIMER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

