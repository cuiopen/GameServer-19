
#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include "SpinLockImpl.h"
#include "Noncopyable.h"
#include "Thread.h"

namespace buf
{

// os specific
#define MAX_BUSY_LOOPS 1000
#ifdef __linux__
#define USE_SCHED_YIELD // XXX:
#endif

class SpinLock : public Noncopyable
{
public:
    SpinLock()
    {
        __spinlock_init(&_lock);
    }

    ~SpinLock() {}

    inline void lock(void)
    {
        if (!trylock())
        {
            int tryn = 0;
            while (__spinlock_locked(&_lock) ||
                    !__spinlock_try_lock(&_lock))
            {
                if (++tryn >= MAX_BUSY_LOOPS)
                    yeild_cpu();
            }
        }
    }

    inline bool trylock(void)
    {
        if (!__spinlock_locked(&_lock))
            return __spinlock_try_lock(&_lock);
        return false;
    }

    inline void unlock(void)
    {
        __spinlock_unlock(&_lock);
    }

private:
    void yeild_cpu()
    {
#ifdef USE_SCHED_YIELD
        sched_yield();
#else
        Thread::usleep(2);
#endif
    }

private:
    __spinlock_t _lock;
};

class ScopeSpinLock
{
public:
    ScopeSpinLock(SpinLock& lock) : _lock(lock)
    {
        _lock.lock();
    }

    ~ScopeSpinLock()
    {
        _lock.unlock();
    }

private:
    SpinLock& _lock;
};

typedef SpinLock FastLock;
typedef ScopeSpinLock ScopeFastLock;

} // namespace buf

#endif // SPINLOCK_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

