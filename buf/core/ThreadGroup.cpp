
#include "Config.h"
#include "ThreadGroup.h"

#if defined(CXX0X) && CXX0X
#include <algorithm>
#endif

namespace buf
{

bool ThreadGroup::put(const Thread* thread)
{
    if (!thread)
        return false;
    ScopeLock lock(_lock);
    if (_size >= _max)
        return false;
    _container[_size++] = const_cast<Thread*>(thread);
    return true;
}

const Thread* ThreadGroup::get(threadid_t id) const
{
    ScopeLock lock(_lock);
    for (size_type i = 0; i < _size; ++i)
        if (_container[i]->tid() == id)
            return _container[i];
    return 0;
}

const Thread* ThreadGroup::operator[](size_type index) const
{
    ScopeLock lock(_lock);
    if (index > _size)
        return 0;
    const_iterator it = _container.begin();
    std::advance(it, index);
    if (it != _container.end())
        return *it;
    return 0;
}

Thread* ThreadGroup::operator[](size_type index)
{
    ScopeLock lock(_lock);
    if (index > _size)
        return 0;
    const_iterator it = _container.begin();
    std::advance(it, index);
    if (it != _container.end())
        return *it;
    return 0;
}

bool ThreadGroup::erase(const Thread* thread)
{
    ScopeLock lock(_lock);
    for (iterator it = _container.begin(), end = _container.end();
            (*it) && it != end; ++it)
    {
        if ((*it) == thread)
        {
            _container.erase(it);
            --_size;
            return true;
        }
    }
    return false;
}

bool ThreadGroup::erase(const char* threadname)
{
    if (!threadname)
        return false;
    ScopeLock lock(_lock);
    for (iterator it = _container.begin(), end = _container.end();
            (*it) && it != end; ++it)
    {
        if ((*it)->getName() == threadname)
        {
            _container.erase(it);
            --_size;
            return true;
        }
    }
    return false;
}

bool ThreadGroup::erase(const pthread_t id)
{
    if (!id)
        return false;
    ScopeLock lock(_lock);
    for (iterator it = _container.begin(), end = _container.end();
            (*it) && it != end; ++it)
    {
        if ((*it)->tid() == id)
        {
            _container.erase(it);
            --_size;
            return true;
        }
    }
    return false;
}

void ThreadGroup::startAll()
{
    ScopeLock lock(_lock);
#if defined(CXX0X) && CXX0X
    std::for_each(_container.begin(), _container.end(),
            [](Thread* t) { if(t && !t->isAlive()) t->start(); });
#else
    for (size_type i=0; i<_size; ++i)
    {
        if (!_container[i]->isAlive())
            _container[i]->start();
    }
#endif
}

void ThreadGroup::joinAll()
{
    ScopeLock lock(_lock);
#if defined(CXX0X) && CXX0X
    std::for_each(_container.begin(), _container.end(),
            [](Thread* t) { if(t) t->join(); });
#else
    for (size_type i=0; i<_size; ++i)
    {
        _container[i]->join();
    }
#endif
}

void ThreadGroup::stopAll()
{
    ScopeLock lock(_lock);
#if defined(CXX0X) && CXX0X
    std::for_each(_container.begin(), _container.end(),
            [](Thread* t) { if(t && t->isAlive()) t->final(); });
#else
    for (size_type i=0; i<_size; ++i)
    {
        if (_container[i]->isAlive())
            _container[i]->final();
    }
#endif
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

