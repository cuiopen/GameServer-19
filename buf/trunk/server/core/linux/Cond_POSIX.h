
#ifndef COND_POSIX_H_
#define COND_POSIX_H_

#include "../Mutex.h"
#include <pthread.h>
#include <errno.h>
#include <stdio.h>

namespace buf
{
    
class CondImpl
{
public:
    CondImpl()
    {
        ::pthread_condattr_init(&_attr);
        ::pthread_cond_init(&_cond, &_attr);
    }

    ~CondImpl()
    {
        ::pthread_condattr_destroy(&_attr);
        ::pthread_cond_destroy(&_cond);
    }

    // 这三个函数务必配套使用
    bool waitImpl(const Mutex& mutex, const struct timespec* abstime = 0)
    {
        int ret = -1;
        if (!abstime)
        {
            ret = ::pthread_cond_wait(&_cond, const_cast<pthread_mutex_t*>(&mutex._mutex));
        }
        else
        {
            ret = ::pthread_cond_timedwait(&_cond, const_cast<pthread_mutex_t*>(&mutex._mutex), abstime);
        }

        return ret == 0;
    }

    inline void signalImpl()
    {
        ::pthread_cond_signal(&_cond);
    }

    inline void broadcastImpl()
    {
        ::pthread_cond_broadcast(&_cond);
    }

protected:
    pthread_condattr_t _attr;
    pthread_cond_t _cond;
};

} // namespace buf

#endif // COND_H_

