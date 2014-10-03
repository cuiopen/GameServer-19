
#ifndef MASTERSERVER_H_
#define MASTERSERVER_H_

#include "common/CommonServer.h"
#include "core/Mutex.h"

#include <vector>
#include <map>

namespace buf
{

enum
{
    SLAVE_SERVER_STATE_UNKNOE = 0,
    SLAVE_SERVER_STATE_ACK = 1,
    SLAVE_SERVER_STATE_SYNC = 2,
    SLAVE_SERVER_STATE_PROCESS = 3,
    SLAVE_SERVER_STATE_RECYCLE = 4
};

struct SlaveServerInfo
{
    u16_t state; // 0-unknow 1-connected 2-verified 3-closed
    u16_t conns; // connections
    ServerCfgBaseInfo base;

    SlaveServerInfo() : state(SLAVE_SERVER_STATE_UNKNOE), conns(0) {}

    inline bool operator==(const SlaveServerInfo& i) const
    {
        return
#ifdef USE_SERVER_LOGIN
            (base.type == SERVER_LOGIN && i.base.type == SERVER_LOGIN) ||
#endif
#ifdef USE_SERVER_SESSION
            (base.type == SERVER_SESSION && i.base.type == SERVER_SESSION) ||
#endif
            (i.base.port == base.port && i.base.ip == base.ip) ||
            (i.base.type == base.type && i.base.id == base.id);
    }
};

class SlaveServerInfoMgr
{
public:
    typedef std::vector<SlaveServerInfo> ServerInfoVec;
    typedef ServerInfoVec::iterator iterator;

    typedef std::map<s32_t, ServerInfoVec> SlaveContainer;

public:
    SlaveServerInfoMgr() {}
    ~SlaveServerInfoMgr() {}

    bool ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info);
    bool sync(s32_t type, s32_t id);
    bool process(s32_t type, s32_t id);
    bool recycle(s32_t type, s32_t id);

    void addSlaveCfg(s32_t type, const ServerCfgBaseInfo& cfg);

private:
    Mutex _lck;
    SlaveContainer _slavs;
};

class MasterServer : public CommonServer
{
public:
    MasterServer(const char* name) : CommonServer(name, SERVER_MASTER, 1) {}
    virtual ~MasterServer() {}

    virtual bool init();
    virtual bool load();
    virtual bool reload();
    bool ipMask(const char* ip);

    inline bool ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info)
    {
        return _mgr.ack(type, ip, info);
    }

    inline bool sync(s32_t type, s32_t id)
    {
        return _mgr.sync(type, id);
    }

    inline bool process(s32_t type, s32_t id)
    {
        return _mgr.process(type, id);
    }

    inline bool recycle(s32_t type, s32_t id)
    {
        return _mgr.recycle(type, id);
    }

    virtual TcpProcessor* createProcessor(socket_t s, sockaddrin_t* sa);

private:
    SlaveServerInfoMgr _mgr;
};

} // namespace buf

#endif // MASTERSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

