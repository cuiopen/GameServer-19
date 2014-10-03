
#ifndef MUTEX_POSIX_H_
#define MUTEX_POSIX_H_

#include "../Config.h"
#include <pthread.h>

namespace buf
{

class MutexImpl
{
public:
    friend class CondImpl;

    /*
     * PTHREAD_MUTEX_DEFAULT
     * PTHREAD_MUTEX_NORMAL - PTHREAD_MUTEX_FAST_NP
     * PTHREAD_MUTEX_ERRORCHECK - PTHREAD_MUTEX_ERRORCHECK_NP
     * PTHREAD_MUTEX_RECURSIVE - PTHREAD_MUTEX_RECURSIVE_NP
     */
    MutexImpl(const int type = PTHREAD_MUTEX_DEFAULT)
    {
        ::pthread_mutexattr_init(&_attr);
        ::pthread_mutexattr_settype(&_attr, type);
        ::pthread_mutex_init(&_mutex, &_attr);
    }

    ~MutexImpl()
    {
        ::pthread_mutexattr_destroy(&_attr);
        ::pthread_mutex_destroy(&_mutex);
    }

    inline int lockImpl()
    {
        return ::pthread_mutex_lock(&_mutex);
    }

    inline int trylockImpl()
    {
        return ::pthread_mutex_trylock(&_mutex);
    }

    inline int unlockImpl()
    {
        return ::pthread_mutex_unlock(&_mutex);
    }

#if 0
    // 允许在外部做一些更复杂的操作, XXX: 这比较危险
    pthread_mutexattr_t& getMutexImplAttr() { return _attr; }
    pthread_mutex_t& getMutexImpl() { return _mutex; }
#endif

protected:
    pthread_mutexattr_t _attr;
    pthread_mutex_t _mutex;
};

} // namespace buf

#endif // MUTEX_POSIX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

