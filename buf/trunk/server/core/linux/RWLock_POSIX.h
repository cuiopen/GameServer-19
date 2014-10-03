
#ifndef RWLOCK_POSIX_Y_
#define RWLOCK_POSIX_Y_

#include "../Config.h"
#include <pthread.h>

namespace buf
{

class RWLockImpl
{
public:
    RWLockImpl()
    {
        ::pthread_rwlockattr_init(&_attr);
        ::pthread_rwlock_init(&_rwlock, &_attr);
    }

    ~RWLockImpl()
    {
        ::pthread_rwlockattr_destroy(&_attr);
        ::pthread_rwlock_destroy(&_rwlock);
    }

    inline void rdlockImpl()
    {
        ::pthread_rwlock_rdlock(&_rwlock);
    }

    inline void wrlockImpl() {
        ::pthread_rwlock_wrlock(&_rwlock);
    }
    
    inline void unlockImpl() {
        ::pthread_rwlock_unlock(&_rwlock);
    }

private:
    pthread_rwlockattr_t _attr;
    pthread_rwlock_t _rwlock;
};

} // namespace buf

#endif // RWLOCK_POSIX_Y_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

