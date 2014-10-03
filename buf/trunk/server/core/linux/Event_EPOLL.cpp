
#include "Config.h"
#include "Event_EPOLL.h"

namespace buf
{

bool EventManagerImpl::addEventImpl(EventImpl* e, bool mod)
{
    if (!e) return false;
    CLRERR();

    struct epoll_event ev;
    ev.events = e->_events;
    ev.data.ptr = reinterpret_cast<void*>(e);

    int op = EPOLL_CTL_ADD;
    if (mod)
        op = EPOLL_CTL_MOD;

    int ret = epoll_ctl(_epoll, op, e->fdImpl(), &ev);
    if (ret == 0 || errno == EEXIST)
        return true;

    return false;
}

bool EventManagerImpl::delEventImpl(socket_t s)
{
    int ret = epoll_ctl(_epoll, EPOLL_CTL_DEL, s, 0);
    return ret == 0;
}

int EventManagerImpl::_waitImpl(int timeout/*milliseconds*/, std::vector<EventImpl*>* events)
{
    struct epoll_event* pee = &_events.front();
    int ret = TEMP_FAILURE_RETRY(::epoll_wait(_epoll, pee, _max, timeout));
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
        if (!pee->data.fd)
            continue;
        e = reinterpret_cast<EventImpl*>(pee->data.ptr);
        if (!e)
            continue;

        if (e->_deleted)
        {
            DEBUG("deleted: %p\n", e);
            continue;
        }

        if (pee->events & EPOLLERR)
        {
            e->onError();
            continue;
        }

        if (pee->events & e->_events)
        {
            if (!pev)
                e->onEvent(pee->events); // XXX: if onEvent() return false then ???
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
    TEMP_FAILURE_RETRY(::close(_epoll));
    _events.clear();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

