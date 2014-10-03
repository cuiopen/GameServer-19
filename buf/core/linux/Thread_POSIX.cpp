
#include "Thread_POSIX.h"

namespace buf
{

bool ThreadImpl::startImpl()
{
    if (isAliveImpl())
        return true;

    pthread_attr_t attr;
    int ret = -1; 

    ret = pthread_attr_init(&attr);
    if (ret != 0)
        return false;

    if (!_joinable)
    {
        ret = ::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if (ret != 0)
            goto L_start_over;
    }

#if 0
    sched_param param;
    ::pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = priority;
    ::pthread_attr_setschedparam(&attr, &param);
#endif

    ret = ::pthread_create(&_tid, &attr, start_run, this);
    if (ret != 0)
        goto L_start_over;

    _lock.lock();
    while (!isAliveImpl())
        _cond.wait(_lock);
    _lock.unlock();

    ::pthread_attr_destroy(&attr);
    return true;

L_start_over:
    ::pthread_attr_destroy(&attr);
    return false;
}

void* ThreadImpl::start_run(void* arg)
{
    ThreadImpl* thread = reinterpret_cast<ThreadImpl*>(arg);
    if (!thread)
        return 0;

    thread->_lock.lock();
    thread->_alive = true;
    thread->_cond.broadcast(); // notify ok
    thread->_lock.unlock();

    thread->_isrunning = true;
    thread->run();

    thread->_lock.lock();
    if (thread->isAliveImpl())
    {
        thread->finalImpl();
        thread->_alive = false;
    }
    thread->_cond.broadcast(); // notify join
    thread->_lock.unlock();

    if (!thread->isJoinableImpl())
        DELETENULL(thread);

    return 0;
}

void ThreadImpl::joinImpl()
{
    if (isJoinableImpl() && isAliveImpl() && tidImpl() )
    {
        ::pthread_join(tidImpl(), 0);
        _tid = 0;
        _lock.lock();
        while (isAliveImpl())
        {
            _cond.wait(_lock);
        }
        _lock.unlock();
    }
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

