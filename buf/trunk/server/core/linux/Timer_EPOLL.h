
#ifndef TIMER_EPOLL_H_
#define TIMER_EPOLL_H_

#include <sys/epoll.h>

namespace buf
{

class TimerImpl
{
public:
    TimerImpl() : _epoll(-1)
    {
        TEMP_FAILURE_RETRY(_epoll = ::epoll_create(1024));
    }
    ~TimerImpl()
    {
        TEMP_FAILURE_RETRY(::close(_epoll));
    }

    int waitImpl(int usecs)
    {
        if (_epoll == -1)
            return -1;
        struct epoll_event e;
        int ret = ::epoll_wait(_epoll, &e, 1, usecs);
        if (ret < 0 && errno == EINTR)
            return 1;
        else
            return ret;
        return 0;
    }

private:
    int _epoll;
};

} // namespace buf

#endif // TIMER_EPOLL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

