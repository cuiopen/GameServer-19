
#ifndef EVENT_H_
#define EVENT_H_

#if defined(USE_EPOLL) && USE_EPOLL
#include "linux/Event_EPOLL.h"
#elif defined(USE_KQUEUE) && USE_KQUEUE && __APPLE__
#include "mac/Event_KQUEUE.h"
#elif defined(USE_LIBEVENT) && USE_LIBEVENT
#error "no implementation"
#elif defined(USE_LIBEV) && USE_LIBEV
#error "no implementation"
#else
#error "no implementation"
#endif
#include "Mutex.h"

#include <map>

namespace buf
{

#define EVENT_READ  EVENTIMPL_READ 
#define EVENT_WRITE EVENTIMPL_WRITE
#define EVENT_RDWR  (EVENTIMPL_READ|EVENTIMPL_WRITE)
#define EVENT_ERROR EVENTIMPL_ERROR
#define EVENT_ALL   (EVENT_READ|EVENT_WRITE|EVENT_ERROR)

class Event : public EventImpl
{
public:
    Event(socket_t s, event_t events) : EventImpl(s, events) {}
    Event(Socket* s, event_t events) : EventImpl(s, events) {}
    virtual ~Event() {}

    inline socket_t fd() const { return fdImpl(); }
    inline event_t events() const { return eventsImpl(); } // listened
    inline void deleted() { _deleted = true; }

    virtual void onError()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    // XXX:
    // EPOLL无法通过事件知道是否有EOF事件
    // KQUEUE
    virtual void onEof()
    {
        DEBUG("%s\n", __PRETTY_FUNCTION__);
    }

    virtual bool onEvent(event_t ev)
    {
        DEBUG("%s: %u\n", __PRETTY_FUNCTION__, ev);
        return false;
    }
};

class EventManager : public EventManagerImpl, public Noncopyable
{
public:
    typedef std::map<socket_t, Event*> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    static const int WAIT_QUEUE = 3200;
    static const int WAIT_TIMEOUT = 1000; // milliseconds

public:
    explicit EventManager(int maxqueue = WAIT_QUEUE)
        : EventManagerImpl(maxqueue) {}
    ~EventManager() { exit(); }

    bool addEvent(Event* e);
    bool modEvent(Event* e);
    bool delEvent(socket_t s);

    inline bool delEvent(Socket* s)
    {
        if (s)
            return delEvent(s->fd());
        return false;
    }

    // XXX: will loop for calling wait()
    // timeout  -1 - indefinitely
    //          0  - return immediately even if no events are  available
    //          >0 - timeout milliseconds
    inline int wait(int timeout = WAIT_TIMEOUT) { return waitImpl(timeout); }

    template <typename E>
    inline  int wait(std::vector<E*>& events/*OUT*/, int timeout = WAIT_TIMEOUT)
    {
        return waitImpl(timeout, events);
    }

    void exit();

private:
    container _events;
    Mutex _lock;
};

} // namespace buf

#endif // EVENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

