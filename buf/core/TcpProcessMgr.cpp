
#include "TcpProcessMgr.h"

namespace buf
{

bool TcpProcessMgr::empty() const
{
    ScopeRDLock lock(_rwlock);
    return _tasks.empty();
}

TcpProcessMgr::size_type TcpProcessMgr::size() const
{
    ScopeRDLock lock(_rwlock);
    return _tasks.size();
}

bool TcpProcessMgr::add(TcpProcess* tp)
{
    if (!tp)
        return false;
    //fprintf(stderr, "%s: %p, %d\n", __PRETTY_FUNCTION__, tp, tp->id());
    ScopeRWLock lock(_rwlock);
    iterator i = _tasks.find(tp->id());
    if (i != _tasks.end())
    {
        //fprintf(stderr, "%s: false\n", __PRETTY_FUNCTION__);
        return false;
    }
    //fprintf(stderr, "%s: true\n", __PRETTY_FUNCTION__);
    return _tasks.insert(value_type(tp->id(), tp)).second;
}

bool TcpProcessMgr::remove(TcpProcess* tp)
{
    if (!tp)
        return false;
    //fprintf(stderr, "%s: %p, %d\n", __PRETTY_FUNCTION__, tp, tp->id());
    ScopeRWLock lock(_rwlock);
    iterator i = _tasks.find(tp->id());
    if (i == _tasks.end())
    {
        //fprintf(stderr, "%s: false\n", __PRETTY_FUNCTION__);
        return false;
    }
    _tasks.erase(tp->id());
    //fprintf(stderr, "%s: true\n", __PRETTY_FUNCTION__);
    return true;
}

void TcpProcessMgr::enumerate(Enumerator<TcpProcess>& enumerator)
{
    _rwlock.rdlock();
    for (iterator i = _tasks.begin(), e = _tasks.end(); i != e; ++i)
    {
        if (!enumerator(i->second))
            break;
    }
    _rwlock.unlock();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

