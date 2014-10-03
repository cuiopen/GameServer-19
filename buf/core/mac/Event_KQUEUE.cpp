
#include "Config.h"
#include "Event_KQUEUE.h"

namespace buf
{

bool EventManagerImpl::addEventImpl(EventImpl* e, bool mod)
{
    DEBUG("addevent: %p\n", e);
    if (!e) return false;
    CLRERR();

    int c = 0;
    // XXX: like level-triggered in epoll
    // if |EV_CLEAR then like edge-triggered in epoll
    // WE USE EV_ADD BY DEFAULT
#if defined(USE_EVENT_ET) && USE_EVENT_ET
    uint16_t flags = EV_ADD|EV_CLEAR;
#else
    uint16_t flags = EV_ADD;
#endif
#ifdef CPU_64BIT
    kqueue_event changes[2] = {{0,0,0,0,0,0,{0,0}},};
    if (e->_events & EVENTIMPL_READ)
    {
        EV_SET64(&changes[c], e->fdImpl(), EVFILT_READ, flags, 0, 0, reinterpret_cast<u64_t>(e), 0, 0);
        ++c;
    }
    if (e->_events & EVENTIMPL_WRITE)
    {
        EV_SET64(&changes[c], e->fdImpl(), EVFILT_WRITE, flags, 0, 0, reinterpret_cast<u64_t>(e), 0, 0);
        ++c;
    }
#else
    kqueue_event changes[2] = {{0,0,0,0,0,0},};
    if (e->_events & EVENTIMPL_READ)
    {
        EV_SET(&changes[c], e->fdImpl(), EVFILT_READ, flags, 0, 0, e);
        ++c;
    }
    if (e->_events & EVENTIMPL_WRITE)
    {
        EV_SET(&changes[c], e->fdImpl(), EVFILT_WRITE, flags, 0, 0, e);
        ++c;
    }
#endif

#ifdef CPU_64BIT
    int ret = kevent64(_kqueue, changes, c, NULL, 0, 0, NULL);
#else
    int ret = kevent(_kqueue, changes, c, NULL, 0, NULL);
#endif
    if (ret == 0 || errno == EEXIST)
        return true;

    return false;
}

bool EventManagerImpl::delEventImpl(socket_t s)
{
#ifdef CPU_64BIT
    kqueue_event changes = {0,0,0,0,0,0,{0,0}};
    EV_SET64(&changes, s, EVFILT_READ, EV_DELETE, 0, 0, 0, 0, 0);
    int ret = kevent64(_kqueue, &changes, 1, NULL, 0, 0, NULL);

    kqueue_event changes1 = {0,0,0,0,0,0,{0,0}};
    EV_SET64(&changes1, s, EVFILT_WRITE, EV_DELETE, 0, 0, 0, 0, 0);
    int ret1 = kevent64(_kqueue, &changes1, 1, NULL, 0, 0, NULL);
#else
    kqueue_event changes = {0,0,0,0,0,0};
    EV_SET(&changes, s, EVFILT_READ, EV_DELETE, 0, 0, 0);
    int ret = kevent(_kqueue, &changes, 1, NULL, 0, NULL);

    kqueue_event changes1 = {0,0,0,0,0,0};
    EV_SET(&changes1, s, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    int ret1 = kevent(_kqueue, &changes1, 1, NULL, 0, NULL);
#endif
    return ret == 0 || ret1 == 1;
}

int EventManagerImpl::_waitImpl(int timeout/*milliseconds*/, std::vector<EventImpl*>* events)
{
    struct timespec ts = {timeout/1000, (timeout%1000)*1000000LL};
    kqueue_event* pee = &_events.front();
#ifdef CPU_64BIT
    int ret = TEMP_FAILURE_RETRY(::kevent64(_kqueue, NULL, 0, pee, _max, 0, &ts));
#else
    int ret = TEMP_FAILURE_RETRY(::kevent(_kqueue, NULL, 0, pee, _max, &ts));
#endif
    if (ret <= 0)
        return ret;

    EventImpl* e = 0;
    EventImpl** pev = 0;
    if (events)
    {
        events->reserve(ret);
        pev = &events->front(); // be more faster;
    }

    int nret = 0;
    for (int i = 0; i < ret; ++i)
    {
        if (!pee)
            continue;
        if (!pee->ident)
            continue;
        e = reinterpret_cast<EventImpl*>(pee->udata);
        if (!e)
            continue;

        if (e->_deleted)
        {
            DEBUG("deleted: %p\n", e);
            continue;
        }

        if (pee->flags & EV_ERROR)
        {
            e->onError();
            continue;
        }

        if (pee->flags & EV_EOF)
        {
            e->onEof();
            continue;
        }

        bool r = (pee->filter==EVFILT_READ);
        bool w = (pee->filter==EVFILT_WRITE);

        if ((r && (e->_events&EVENTIMPL_READ)) ||
             w && (e->_events&EVENTIMPL_WRITE))
        {
            event_t ev = 0;
            if (r)
                ev = (e->_events&EVENTIMPL_READ);
            if (w)
                ev |= (e->_events&EVENTIMPL_WRITE);

            if (!pev)
                e->onEvent(ev); // XXX: if onEvent() return false then ???
            else
                *pev++ = e;

            ++nret;
        }

        ++pee;
    }

    return nret;
}

void EventManagerImpl::exitImpl()
{
    TEMP_FAILURE_RETRY(::close(_kqueue));
    _events.clear();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

