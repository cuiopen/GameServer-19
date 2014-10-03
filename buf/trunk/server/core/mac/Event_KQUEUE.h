
#ifndef EVENT_KQUEUE_H_
#define EVENT_KQUEUE_H_

#include "../Noncopyable.h"
#include "../Socket.h" // for socket_t

#include <vector>
#include <sys/event.h> // for kqueue,kevent
#include <unistd.h> // for TEMP_FAILURE_RETRY
#include <errno.h> // for errno, EINTR

namespace buf
{

typedef unsigned int event_t; // event container
typedef unsigned int interval_t;

#define EVENTIMPL_READ  ((event_t)(0x01)) // XXX: fd must be nonblocked
#define EVENTIMPL_WRITE ((event_t)(0x02))
#define EVENTIMPL_ERROR ((event_t)(0x00))

#ifdef CPU_64BIT
typedef struct kevent64_s kqueue_event;
#else
typedef struct kevent kqueue_event;
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
        _kqueue = kqueue();
        _events.resize(_max);
    }

    ~EventManagerImpl() { exitImpl(); }

    bool addEventImpl(EventImpl* e, bool mod = false);
    bool delEventImpl(socket_t s);

    inline int waitImpl(int timeout/*milliseconds*/)
    {
        return _waitImpl(timeout);
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
    socket_t _kqueue;
    std::vector<kqueue_event> _events;
    int _max; // be more faster than _events.size()
};

} // namespace buf

#endif // EVENT_KQUEUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

