
#ifndef TCPPROCESSMGR_H_
#define TCPPROCESSMGR_H_

#include "TcpProcess.h"
#include "Singleton.h"
#include "RWLock.h"
#include "Enumerator.h"
#include "UnorderedContainer.h"

namespace buf
{

class TcpProcessMgr : public Singleton<TcpProcessMgr>
{
public:
    //typedef UNORDERED_MAP<u32_t, TcpProcess*> ProcessMap_t;
    typedef std::map<u32_t, TcpProcess*> ProcessMap_t;
    typedef ProcessMap_t::iterator iterator;
    typedef ProcessMap_t::const_iterator const_iterator;
    typedef ProcessMap_t::value_type value_type;
    typedef ProcessMap_t::size_type size_type;

public:
    TcpProcessMgr() {}
    virtual ~TcpProcessMgr() {}

    bool empty() const;
    size_type size() const;
    bool add(TcpProcess* tp);
    bool remove(TcpProcess* tp);
    void enumerate(Enumerator<TcpProcess>& enumerator);

private:
    mutable RWLock _rwlock;
    ProcessMap_t _tasks;
};

} // namespace buf

#endif // TCPPROCESSMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

