
#ifndef SESSIONTIMER_H_
#define SESSIONTIMER_H_

#include "core/Timer.h"

namespace buf
{

class DoProcessTimer : public Timer
{
public:
    DoProcessTimer(u32_t interval, u32_t count = UNLIMITED)
        : Timer(interval, count) {}
    virtual ~DoProcessTimer(){}
    
    virtual bool onTimer();
};

class SessionTimer : public TimerThread
{
public:
    void init();
};

} // namespace buf

#endif // SESSIONTIMER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

