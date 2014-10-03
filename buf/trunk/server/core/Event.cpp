
#include "Event.h"

namespace buf
{

bool EventManager::addEvent(Event* e)
{
    if (!e)
        return false;

    //fprintf(stderr, "%s: %p(%d)\n", __PRETTY_FUNCTION__, e, e->fd());
    _lock.lock();
    iterator i = _events.find(e->fd());
    if (i != _events.end())
    {
        _lock.unlock();
        //fprintf(stderr, "%s: %p(%d) => false\n", __PRETTY_FUNCTION__, e, e->fd());
        return false;
    }

    bool ret = false; 
    if (addEventImpl(e))
        ret = _events.insert(std::make_pair(e->fd(), e)).second;

    _lock.unlock();
    if (!ret)
    {
        delEvent(e->fd());
        DELETE(e);
    }
    //fprintf(stderr, "%s: %p(%d) => %s\n", __PRETTY_FUNCTION__, e, e->fd(), ret?"true":"false");
    return ret;
}

bool EventManager::modEvent(Event* e)
{
    _lock.lock();
    iterator i = _events.find(e->fd());
    if (i != _events.end())
    {
        if (addEventImpl(e, true))
        {
            if (i->second && i->second != e)
            {
                DELETE(i->second);
                _events.erase(i);
            }

            bool ret = _events.insert(std::make_pair(e->fd(), e)).second;
            _lock.unlock();
            if (!ret)
            {
                delEvent(e->fd());
                DELETE(e);
            }
            return ret;
        }
    }
    _lock.unlock();

    return addEvent(e);
}

bool EventManager::delEvent(socket_t s)
{
    ScopeLock lock(_lock);
    DEBUG("%s: (%d)\n", __PRETTY_FUNCTION__, s);
    if (delEventImpl(s))
    {
        iterator i = _events.find(s);
        if (i != _events.end())
        {
            static_cast<Event*>(i->second)->deleted();
            // TODO:
            //DELETE(i->second);
            _events.erase(i);
        }
        DEBUG("%s: (%d) => true\n", __PRETTY_FUNCTION__, s);
        return true;
    }
    DEBUG("%s: (%d) => false\n", __PRETTY_FUNCTION__, s);
    return false;
}

void EventManager::exit()
{
    ScopeLock lock(_lock);
    for (iterator i = _events.begin(); i != _events.end(); ++i)
    {
        if (i->second)
        {
            delEventImpl(i->second->fd());
            DELETE(i->second);
        }
    }

    exitImpl();
    _events.clear();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

