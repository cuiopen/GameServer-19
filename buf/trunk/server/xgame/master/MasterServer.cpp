
#include "MasterServer.h"
#include "MasterProcessor.h"
#include "common/Cfg.h"
#include "script/Script.h"

namespace buf
{

bool SlaveServerInfoMgr::ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info)
{
    if (!ip)
        return false;

    ScopeLock lck(_lck);
    SlaveContainer::iterator sci = _slavs.find(type);
    if (sci == _slavs.end())
        return false;
    ServerInfoVec& siv = sci->second;

    u16_t n = 0;
    for (iterator i = siv.begin(); i != siv.end(); ++i)
    {
        ++n;

        if ((*i).base.ip == ip && (*i).state == SLAVE_SERVER_STATE_UNKNOE)
        {
            (*i).state = SLAVE_SERVER_STATE_ACK;
            (*i).base.id = n;
            info = &(*i).base;
            return true;
        }
    }
    return false;
}

bool SlaveServerInfoMgr::sync(s32_t type, s32_t id)
{
    ScopeLock lck(_lck);
    SlaveContainer::iterator sci = _slavs.find(type);
    if (sci == _slavs.end())
        return false;
    ServerInfoVec& siv = sci->second;
    u16_t n = 0;
    for (iterator i = siv.begin(); i != siv.end(); ++i)
    {
        ++n;

        if (n == id && (*i).state == SLAVE_SERVER_STATE_ACK)
        {
            (*i).state = SLAVE_SERVER_STATE_SYNC;
            return true;
        }
    }
    return false;
}

bool SlaveServerInfoMgr::process(s32_t type, s32_t id)
{
    ScopeLock lck(_lck);
    SlaveContainer::iterator sci = _slavs.find(type);
    if (sci == _slavs.end())
        return false;
    ServerInfoVec& siv = sci->second;
    u16_t n = 0;
    for (iterator i = siv.begin(); i != siv.end(); ++i)
    {
        ++n;

        if (n == id && (*i).state == SLAVE_SERVER_STATE_SYNC)
        {
            (*i).state = SLAVE_SERVER_STATE_PROCESS;
            return true;
        }
    }
    return false;
}

bool SlaveServerInfoMgr::recycle(s32_t type, s32_t id)
{
    ScopeLock lck(_lck);
    SlaveContainer::iterator sci = _slavs.find(type);
    if (sci == _slavs.end())
        return false;
    ServerInfoVec& siv = sci->second;
    u16_t n = 0;
    for (iterator i = siv.begin(); i != siv.end(); ++i)
    {
        ++n;

        if (n == id)
        {
            (*i).state = SLAVE_SERVER_STATE_UNKNOE;
            return true;
        }
    }
    return false;
}

void SlaveServerInfoMgr::addSlaveCfg(s32_t type, const ServerCfgBaseInfo& cfg)
{
    ScopeLock lck(_lck);
    ServerInfoVec& siv = _slavs[type];

    SlaveServerInfo info;
    info.base = cfg;

    for (iterator i = siv.begin(); i != siv.end(); ++i)
    {
        if (*i == info)
        {
            sLog.warn("There is conflict in configuration.\n");
            sLog.warn("type: %u\n", cfg.type);
            sLog.warn("id: %u\n", cfg.id);
            sLog.warn("ip: %s\n", cfg.ip.c_str());
            sLog.warn("port: %u\n", cfg.port);
            sLog.warn("extip: %s\n", cfg.extip.c_str());
            sLog.warn("extport: %u\n", cfg.extport);
            return;
        }
    }

    siv.push_back(info);
    return;
}

bool MasterServer::load()
{
#ifdef USE_SERVER_LOGIN
    // login
    {
        Table login = gCfg.get<Table>(getServerTypeString(SERVER_LOGIN));
        ServerCfgBaseInfo info(SERVER_LOGIN);
        getServerCfgBaseInfo(login, info);
        _mgr.addSlaveCfg(SERVER_LOGIN, info);
    }
#endif

#ifdef USE_SERVER_GATEWAY
    // gateway
    {
        Table gateway = gCfg.get<Table>(getServerTypeString(SERVER_GATEWAY));
        s16_t size = gateway.size();
        for (s16_t i = 0; i < size; ++i)
        {
            ServerCfgBaseInfo info(SERVER_GATEWAY);
            Table g = gateway.get<Table>(i+1);
            getServerCfgBaseInfo(g, info);
            info.id = i+1;
            _mgr.addSlaveCfg(SERVER_GATEWAY, info);
        }
    }
#endif

#ifdef USE_SERVER_SESSION
    // session
    {
        Table session = gCfg.get<Table>(getServerTypeString(SERVER_SESSION));
        ServerCfgBaseInfo info(SERVER_SESSION);
        getServerCfgBaseInfo(session, info);
        _mgr.addSlaveCfg(SERVER_SESSION, info);
    }
#endif

    // game
    {
        Table game = gCfg.get<Table>(getServerTypeString(SERVER_GAME));
        s16_t size = game.size();
        for (s16_t i = 0; i < size; ++i)
        {
            ServerCfgBaseInfo info(SERVER_GAME);
            Table s = game.get<Table>(i+1);
            getServerCfgBaseInfo(s, info);
            info.id = i+1;
            _mgr.addSlaveCfg(SERVER_GAME, info);
        }
    }

    // record 
    {
        Table record = gCfg.get<Table>(getServerTypeString(SERVER_RECORD));
        s16_t size = record.size();
        for (s16_t i = 0; i < size; ++i)
        {
            ServerCfgBaseInfo info(SERVER_RECORD);
            Table s = record.get<Table>(i+1);
            getServerCfgBaseInfo(s, info);
            info.id = i+1;
            _mgr.addSlaveCfg(SERVER_RECORD, info);
        }
    }

    return true;
}

bool MasterServer::reload()
{
    return load();
}

bool MasterServer::init()
{
    ServerCfgBaseInfo info(getType());
    Table master = gCfg.get<Table>(getServerTypeString(getType()));
    if (!getServerCfgBaseInfo(master, info))
        return false;

    if (!CommonServer::init(info))
        return false;

    Table mask = master.get<Table>("mask");
    getIpMask(mask);

    if (!load())
        return false;
    return true;
}

bool MasterServer::ipMask(const char* ip)
{
    if (!ip)
        return false;
    return gCfg.isIPAllowed(ip);
}

TcpProcessor* MasterServer::createProcessor(socket_t s, sockaddrin_t* sa)
{
    if (s < 0 || !sa)
        return 0;
    sLog.trace("New MasterProcessor with: %d\n", s);
    return new MasterProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

