
#ifndef TIMER_KQUEUE_H_
#define TIMER_KQUEUE_H_

#include "Event_KQUEUE.h"

namespace buf
{

class TimerImpl
{
public:
    TimerImpl() : _kqueue(-1)
    {
        TEMP_FAILURE_RETRY(_kqueue = ::kqueue());
    }
    ~TimerImpl()
    {
        if (_kqueue > 0)
            TEMP_FAILURE_RETRY(::close(_kqueue));
    }

    int waitImpl(int usecs)
    {
        if (_kqueue < 0)
            return -1;
        struct timespec ts = {usecs/1000, (usecs%1000)*1000000LL};
#ifdef CPU_64BIT
        kqueue_event changes[1] = {{0,0,0,0,0,0,{0,0}},};
        int ret = ::kevent64(_kqueue, NULL, 0, &changes[0], 1, 0, &ts);
#else
        kqueue_event changes[1] = {{0,0,0,0,0,0},};
        int ret = ::kevent(_kqueue, NULL, 0, &changes[0], 1, ts);
#endif
        if (ret < 0 && errno == EINTR)
            return 1;
        else
            return ret;
        return 0;
    }

private:
    int _kqueue;
};

} // namespace buf

#endif // TIMER_KQUEUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

