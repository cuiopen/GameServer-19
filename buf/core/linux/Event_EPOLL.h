
#ifndef EVENT_EPOLL_H_
#define EVENT_EPOLL_H_

#include "../Noncopyable.h"
#include "../Socket.h" // for socket_t

#include <vector>
#include <sys/epoll.h> // for EPOLL*
#include <unistd.h> // for TEMP_FAILURE_RETRY
#include <errno.h> // for errno, EINTR

namespace buf
{

typedef unsigned int event_t; // event container
typedef unsigned int interval_t;

#if defined(USE_EVENT_ET) && USE_EVENT_ET
#define EVENTIMPL_READ ((event_t)(EPOLLIN|EPOLLET)) // XXX: fd must be nonblocked
#define EVENTIMPL_WRITE ((event_t)EPOLLOUT|EPOLLET)
#define EVENTIMPL_ERROR ((event_t)(EPOLLERR|EPOLLPRI|EPOLLET))
#else
#define EVENTIMPL_READ ((event_t)EPOLLIN) // XXX: fd must be nonblocked
#define EVENTIMPL_WRITE ((event_t)EPOLLOUT)
#define EVENTIMPL_ERROR ((event_t)(EPOLLERR|EPOLLPRI))
#endif

class EventImpl
{
public:
    friend class EventManagerImpl;

public:
    EventImpl(socket_t s, event_t event)
        : _deleted(false), _isfd(true), _events(event)
    {
         _s.fd = s;
    }

    EventImpl(Socket* s, event_t event)
        : _deleted(false), _isfd(false), _events(event)
    {
         _s.s = s;
    }

    virtual ~EventImpl()
    {
        _s.s = 0;
        _s.fd = 0;
    }

    socket_t fdImpl() const
    {
        if (_isfd)
            return _s.fd;
        if (_s.s)
            return _s.s->fd();
        return -1;
    }

    inline event_t eventsImpl() const { return _events; }

    virtual void onError() = 0;
    virtual void onEof() = 0;
    virtual bool onEvent(event_t ev) = 0;

protected:
    bool _deleted;
    bool _isfd;
    union s
    {
        socket_t fd;
        Socket* s;
    } _s;
    event_t _events; // XXX: events listened
};

class EventManagerImpl
{
public:
    explicit EventManagerImpl(int maxqueue) : _max(maxqueue)
    {
        _epoll = epoll_create(_max);
        _events.resize(_max);
    }

    ~EventManagerImpl() { exitImpl(); }

    bool addEventImpl(EventImpl* e, bool mod = false);
    bool delEventImpl(socket_t s);

    inline int waitImpl(int timeout/*milliseconds*/)
    {
        return _waitImpl(timeout, NULL);
    }

    template <typename E>
    inline int waitImpl(int timeout/*milliseconds*/, std::vector<E*>& events)
    {
        return _waitImpl(timeout, reinterpret_cast<std::vector<EventImpl*>*>(&events));
    }

    void exitImpl();

private:
    int _waitImpl(int timeout/*milliseconds*/, std::vector<EventImpl*>* events = NULL);

private:
    socket_t _epoll;
    std::vector<struct epoll_event> _events;
    int _max; // be more faster than _events.size()
};

} // namespace buf

#endif // EVENT_EPOLL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

