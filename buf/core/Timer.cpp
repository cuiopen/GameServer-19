
#include "Timer.h"
#include "RTime.h"

namespace buf
{

void TimerManager::signal(Timer* timer)
{
    TimerMinHeap_t::pointer n = _heap.top();
    if (n && n->getVal() == timer)
    {
        // TODO: if we has a timer less then per 1000ms then we do not need this
    }
}

bool TimerManager::addTimer(Timer* timer)
{
    if (!timer)
        return false;

    ScopeLock lk(_lock);
    // when time running backwards

    if (!timer->setOwner(_heap.push(timer)))
        return false;

    signal(timer);
    return true;
}

bool TimerManager::delTimer(Timer* timer)
{
    if (!timer)
        return false;

    ScopeLock lk(_lock);

    signal(timer);
    bool ret = _heap.erase(reinterpret_cast<TimerMinHeap_t::pointer>(timer->getOwner())) == 0;
    if (ret)
        DELETE(timer);
    return ret;
}

void TimerManager::correct_time(unsigned int usecs)
{
    // TODO:
}

int TimerManager::wait()
{
    if (_heap.empty())
    {
        Thread::msleep(IDL_EMPTY_TIME);
        return 0;
    }

    TimerMinHeap_t::pointer n = 0;
    {
        ScopeLock lk(_lock);
        n = _heap.pop();
        if (!n)
            return 0;
    }

    Timer* timer = n->getVal();
    if (!timer)
    {
        DELETE(n);
        return 0;
    }

    RTime now;
    const RTime& rt = timer->getTimeout();
    if (now < rt)
    {
#ifndef _NDEBUG
        {
            //TimeFilm tf;
#endif
            int elapse = now.elapse(rt);
            // XXX:
            //  0   - normal
            //  1   - interrupted
            // -1   - error
            int ret = waitImpl(elapse);
            if (ret < 0)
                return ret;

            if (ret == 1)
            {
                RTime yan;
                int yae = now.elapse(yan);
                if (yae < elapse)
                {
                    now.now(elapse - yae);
                    timer->setTimeout(now); // XXX: set timeout before onTimer
                }
                {
                    DEBUG("just push the timer back...\n");
                    ScopeLock lk(_lock);
                    timer->setOwner(_heap.push(n));
                }
                return 0;
            }
#ifndef _NDEBUG
        }
#endif
    }

    now.now(timer->getInterval());

    // XXX: 在定时器处理定时操作之前已确定下一次操作的时间
    //      如果定时操作函数onTimer()处理时间超过定时时长
    //      当这一次onTimer()处理完后便会调起下一次onTimer()
    timer->setTimeout(now); // XXX: set timeout before onTimer
    {
        // TODO:
        // TimeFilm tf;
        timer->onTimer();
    }
    timer->decCount();

    if (timer->getCount())
    {
        ScopeLock lk(_lock);
        timer->setOwner(_heap.push(n));
    }
    else
    {
        DELETE(timer);
        DELETE(n);
    }

    return 0;
}

void TimerManager::clear()
{
    ScopeLock lk(_lock);
    while (true)
    {
        TimerMinHeap_t::pointer n = _heap.pop();
        if (!n)
            break;

        Timer* timer = n->getVal();
        DELETE(n);
        if (!timer)
            continue;

        DELETE(timer);
    }
}

void TimerThread::run()
{
    while (!isFinal())
    {
        if (wait() < 0)
        {
            final();
            break;
        }
    }
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

