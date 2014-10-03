
#ifndef MUTEX_H_
#define MUTEX_H_

#include "Config.h"
#include "Noncopyable.h"

#ifdef UNIXLIKE
#include "linux/Mutex_POSIX.h"
#endif

namespace buf
{

class Mutex : public MutexImpl, public Noncopyable
{
public:
    friend class CondImpl;

    Mutex() : MutexImpl() {}
    ~Mutex() {}

    inline int lock() { return lockImpl(); }
    inline int trylock() { return trylockImpl(); }
    inline int unlock() { return unlockImpl(); }
};

class ScopeMutex : public Noncopyable
{
public:
    ScopeMutex(Mutex& lock) : _mutex(lock)
    {
        _mutex.lock();
    }

    ~ScopeMutex()
    {
        _mutex.unlock();
    }

private:
    Mutex& _mutex;
};

typedef ScopeMutex ScopeLock;
typedef ScopeMutex AutoLock;

} // namespace buf

#endif // MUTEX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

