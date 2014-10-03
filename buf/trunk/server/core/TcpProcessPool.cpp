
#include "Config.h"
#include "TcpProcessPool.h"
#include "Logger.h"
#include "System.h"

#include <algorithm>

namespace buf
{

// [[ TcpProcessPool
TcpProcessPool::TcpProcessPool()
{
    MEMZRO(_tgs, sizeof(_tgs));
}

TcpProcessPool::~TcpProcessPool()
{
    closeAll();
}

bool TcpProcessPool::init(const TPConfig config[TPS_MAX])
{
    char name[128] = {0};
    StateThreadGroup* tg = 0;
    StateThread* st = 0;

    for (int i = 0; i < TPS_MAX; ++i)
    {
        int tmax = config[i].tmax;
        if (!tmax)
            continue;

        SNPRINTF(name, sizeof(name), "THREADGROUP-%s", config[i].name);
        tg = BUFNEW StateThreadGroup(name, tmax);
        if (!tg)
            return false;

        for (int j = 0; j < tmax; ++j)
        {
            SNPRINTF(name, sizeof(name), "THREAD-%s-%d", config[i].name, j);

            switch (i)
            {
                case TPS_ACK:
                    st = BUFNEW AckThread(name, static_cast<TcpProcessState>(i), config[i].pmax);
                    break;

                case TPS_SYNC:
                    st = BUFNEW SyncThread(name, static_cast<TcpProcessState>(i), config[i].pmax);
                    break;

                case TPS_PROCESSING:
                    st = BUFNEW ProcessingThread(name, static_cast<TcpProcessState>(i), config[i].pmax);
                    break;

                case TPS_RECYCLE:
                    st = BUFNEW RecycleThread(name, static_cast<TcpProcessState>(i), config[i].pmax);
                    break;

                default:
                    break;
            }

            if (!st)
            {
                DELETENULL(tg);
                return false;
            }

            if (!tg->put(st)) // XXX: put into thread group
            {
                DELETENULL(st);
                DELETENULL(tg);
                return false;
            }

            st = 0;
        }

        _tgs[i] = tg;
        tg = 0;
    }

    return true;
}

bool StateThreadGroup::add(const TcpProcessPool* pool, const TcpProcess* tp)
{
    if (!pool || !tp)
        return false;

    StateThread* st = 0;
    TcpProcessState state = tp->getState();
    switch (state)
    {
        case TPS_ACK:
            static unsigned long int hashval = 0;
            st = static_cast<StateThread*>((*this)[hashval++%size()]);
            break;

        default:
            {
                for (ThreadGroup::iterator i = begin(); i != end(); ++i)
                {
                    st = static_cast<StateThread*>(*i);
                    if (st && st->rsize())
                        break;
                    else
                        st = 0;
                }
            }
            break;
    }

    if (!st)
        return false;

    if (!st->isAlive())
    {
        if (!st->start())
            return false;
    }

    const_cast<TcpProcess*>(tp)->setPool(pool);
    if (st->isAlive())
        st->add(tp);

    return true;
}

// XXX: It's must be locked when more than one I/O thread
bool TcpProcessPool::add(const TcpProcess* tp)
{
    if (!tp || !_tgs[0])
        return false;
    TcpProcessState state = tp->getState();
    if (state >= TPS_MAX)
        return false;
    if (!_tgs[state]->add(this, tp))
        DELETE(tp);
    return true;
}

void TcpProcessPool::closeAll()
{
    for (int i = 0; i < TPS_MAX; ++i)
        DELETENULL(_tgs[i]);
}

void TcpProcessPool::join()
{
    ThreadGroup* tg = 0;
    for (int i = 0; i < TPS_MAX; ++i)
    {
        tg = _tgs[i];
        if (tg)
        {
            tg->stopAll();
            tg->joinAll();
        }
    }
}
// ]] TcpProcessPool

// [[ StateThread
StateThread::StateThread(const char* name, TcpProcessState state, int maxprocessor)
    : Thread(name), _size(0), _maxprocessor(maxprocessor), _state(state) {} 

StateThread::~StateThread() {}

bool StateThread::tostate(const TcpProcess* tp, TcpProcessState state)
{
    DEBUG("state: tp[%s] param(%s)\n", tp->toString(), TcpProcess::stateString(state));
    DEBUG("state from [%s] to [%s]\n", TcpProcess::stateString(getState()), state==TPS_MAX?tp->toString():TcpProcess::stateString(state));
    if (!tp)
        return false;
    if (tp->getState() == state) // XXX: 避免切换多次
        return false;
    if (state > TPS_MAX)
        return false;
    if (!tp->getPool())
        return false;
    if (state != TPS_MAX)
        const_cast<TcpProcess*>(tp)->setState(state);
    DEBUG("tostate : %s\n", tp->toString());
    if (!tp->getPool()->add(tp))
        DELETE(tp);

    return true;
}
// ]] StateThread

// [[ AckThread
void AckThread::_add(const TcpProcess* tp)
{
    if (!tp)
        return;
    if (!tp->getPool())
        return;

    if (!tp->getPool()->addAckEvent(_em, tp, this) ||
            !const_cast<TcpProcess*>(tp)->uniqueAdd())
    {
        remove(tp);
        if (!recycle(tp))
        {
            const_cast<TcpProcess*>(tp)->onDisconnected();
            DELETENULL(tp);
        }
        return;
    }

    StateThread::_add(tp);
    return;
}

void AckThread::run()
{
    RTime rt;
    while (!isFinal())
    {
        checkQueue();

        if (!_tps.empty())
        {
            RTime now;
            iterator i, n;
            for(i = _tps.begin(), n = i, ++n; i != _tps.end(); i = n, ++n)
            {
                TcpProcess* tp = *i;
                if (tp->checkAckTimeout(now))
                {
                    sLog.error("ACK TIMEOUT(%d): from %s:%u.\n", tp->id(), tp->sock()->getRemoteIp(), tp->sock()->getRemotePort());
                    remove(i);
                    if (!recycle(tp))
                    {
                        tp->onDisconnected();
                        DELETENULL(tp);
                    }
                }
            }
        }

        if (_em.wait(0) < 0)
        { // XXX: system error
            _em.exit();
            final();
            break;
        }
        if (rt.elapse() < TICK)
            Thread::msleep(TICK);
        rt.now();
    }

    checkQueue();
    if (_size)
    {
        iterator i, n;
        for(i = _tps.begin(), n = i, ++n; i != _tps.end(); i = n, ++n)
        {
            TcpProcess* tp = *i;
            remove(i);
            if (!recycle(tp))
            {
                tp->onDisconnected();
                DELETENULL(tp);
            }
        }
    }
}

void AckThread::changeState(const TcpProcess* tp)
{
    if (!tp)
        return;
    if (tp->getState() == getState())
        return;
    if (tp->isSwitching())
        return;
    tp->setSwiching();
    remove(tp);
    tostate(tp);
}
// ]] AckThread

// [[ SyncThread
void SyncThread::run()
{
    RTime rt;
    TcpProcess* tp = 0;
    while (!isFinal())
    {
        checkQueue();
        if (_size)
        {
            for (iterator i = _tps.begin(); i != _tps.end(); )
            {
                tp = *i;
                switch (tp->sync())
                {
                    case 0:
                        ++i;
                        break;

                    case 1:
                        {
                            --_size;
                            i = _tps.erase(i);
                            tostate(tp, TPS_PROCESSING);
                        }
                        break;

                    case -1:
                        {
                            --_size;
                            i = _tps.erase(i);
                            tostate(tp, TPS_RECYCLE);
                        }
                        break;

                    default:
                        ++i;
                        break;
                }
            }
        }

        if (rt.elapse() < TICK)
            Thread::msleep(TICK);
        rt.now();
    }

    checkQueue();
    if (_size)
    {
        iterator i, n;
        for(i = _tps.begin(), n = i, ++n; i != _tps.end(); i = n, ++n)
        {
            TcpProcess* tp = *i;
            remove(i);
            if (!recycle(tp))
            {
                tp->onDisconnected();
                DELETENULL(tp);
            }
        }
    }
}
// ]] SyncThread

// [[ ProcessingThread
void ProcessingThread::_add(const TcpProcess* tp)
{
    if (!tp->getPool())
        return;

    if (!tp->getPool()->addProcessingEvent(_em, tp, this))
    {
        remove(tp);
        if (!recycle(tp))
        {
            const_cast<TcpProcess*>(tp)->onDisconnected();
            DELETENULL(tp);
        }
        return;
    }
    StateThread::_add(tp);
    const_cast<TcpProcess*>(tp)->onEnterProcess();
}

void ProcessingThread::run()
{
    RTime rt;
    while (!isFinal())
    {
        checkQueue();

        if (!_tps.empty())
        {
            RTime now;
            iterator i, n;
            for(i = _tps.begin(), n = i, ++n; i != _tps.end(); i = n, ++n)
            {
                // TODO:
            }
        }

        if (_em.wait(0) < 0)
        { // XXX: system error
            _em.exit();
            final();
            break;
        }
        if (rt.elapse() < TICK)
            Thread::msleep(TICK);
        rt.now();
    }

    checkQueue();
    if (_size)
    {
        iterator i, n;
        for(i = _tps.begin(), n = i, ++n; i != _tps.end(); i = n, ++n)
        {
            TcpProcess* tp = *i;
            remove(i);
            if (!recycle(tp))
            {
                tp->onDisconnected();
                DELETENULL(tp);
            }
        }
    }
}

void ProcessingThread::changeState(const TcpProcess* tp)
{
    if (!tp)
        return;
    if (tp->getState() == getState())
        return;
    if (tp->isSwitching())
        return;
    tp->setSwiching();
    remove(tp);
    tostate(tp);
}
// ]] ProcessingThread

// [[ RecycleThread
void RecycleThread::run()
{
    RTime rt;
    TcpProcess* tp = 0;
    while (!isFinal())
    {
        checkQueue();
        if (_size)
        {
            for (iterator i = _tps.begin(); i != _tps.end(); )
            {
                tp = *i;
                switch (tp->recycle())
                {
                    case 0:
                        // try again
                        break;

                    case 1:
                        {
                            // XXX: do something else
                            --_size;
                            i = _tps.erase(i);
                            tp->uniqueRemove();
                            tp->onDisconnected();
                            DELETENULL(tp);
                        }
                        break;

                    case -1:
                        {
                            --_size;
                            i = _tps.erase(i);
                            tp->uniqueRemove();
                            tp->onDisconnected();
                            DELETENULL(tp);
                        }
                        break;

                    default:
                        ++i;
                        break;
                }
            }
        }
        if (rt.elapse() < TICK)
            Thread::msleep(TICK);
        rt.now();
    }

    checkQueue();
    if (_size)
    {
        for (iterator i = _tps.begin(); i != _tps.end(); ++i)
            DELETE(*i);
        _tps.clear();
    }
}
// ]] RecycleThread

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

