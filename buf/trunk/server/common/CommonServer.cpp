
#include "Cfg.h"
#include "CommonServer.h"
#include "core/Socket.h"

namespace buf
{

bool CommonServer::getServerCfgBaseInfo(Table& cfg, ServerCfgBaseInfo& info)
{
    info.ack = cfg.get<s32_t>("ack");
    info.ack_tasks = cfg.get<s32_t>("ack_tasks");
    info.sync = cfg.get<s32_t>("sync");
    info.processing = cfg.get<s32_t>("processing");
    info.processing_tasks = cfg.get<s32_t>("processing_tasks");
    info.recycle = cfg.get<s32_t>("recycle");

    if (!info.ack) info.ack = 1;
    if (!info.ack_tasks) info.ack_tasks = AckThread::PROCESSORS_PERTHREAD;
    if (!info.sync) info.sync = 1;
    if (!info.processing) info.processing = 1;
    if (!info.processing_tasks) info.processing_tasks = ProcessingThread::PROCESSORS_PERTHREAD;
    if (!info.recycle) info.recycle = 1;

    info.port = cfg.get<port_t>("port");
    info.extport = cfg.get<port_t>("extport");

    const char* ip = cfg.get<const char*>("ip");
    if (ip) info.ip = ip;
    ip = cfg.get<const char*>("extip");
    if (ip) info.extip = ip;

    Table inet = gCfg.get<Table>("inet");
    if (!info.ip.length() || strncmp(info.ip.c_str(), "nil", 3) == 0)
    {
        const char* inner = inet.get<const char*>("inner");
        if (inner && strncmp(inner, "nil", 3) != 0)
            info.ip = Socket::getIpByIf(inner);
    }

    if (!info.extip.length() || strncmp(info.ip.c_str(), "nil", 3) == 0)
    {
        const char* outer = inet.get<const char*>("outer");
        if (outer && strncmp(outer, "nil", 3) != 0)
            info.extip = Socket::getIpByIf(outer);
    }

    return true;
}

bool CommonServer::getIpMask(Table& mask)
{
    if (!mask.size())
        return false;
    for (int i = 1; i <= mask.size(); ++i)
    {
        const char* ipm = mask.get<const char*>(i);
        if (ipm)
            gCfg.setAllowedIP(ipm);
    }
    return false;
}

bool CommonServer::getLogCfg(Table& cfg, CommonServer* cs)
{
    if (!cfg.validate() || !cs)
        return false;

    Table s = cfg.get<Table>("slog");
    if (!s.validate())
        return false;
    sLog.setName(getServerTypeString(cs->getType()));

    const char* slogdir = s.get<const char*>("logdir");
    if (!slogdir)
        slogdir = "~/log";
    if (!sLog.init(slogdir))
        return 0;

    bool sconsole = s.get<bool>("console");
    sLog.setConsole(sconsole);

    bool scolor = s.get<bool>("color");
    sLog.setColored(scolor);

    LoggerLevel_t slvl = s.get<LoggerLevel_t>("level");
    sLog.setLevel(slvl);

    return true;
}

inline void initCfg(const ServerCfgBaseInfo& info, TPConfig cfg[TPS_MAX])
{
    cfg[TPS_ACK].tmax = info.ack;
    cfg[TPS_ACK].pmax = info.ack_tasks;
    cfg[TPS_SYNC].tmax = info.sync;
    cfg[TPS_SYNC].pmax = 1;
    cfg[TPS_PROCESSING].tmax = info.processing;
    cfg[TPS_PROCESSING].pmax = info.processing_tasks;
    cfg[TPS_RECYCLE].tmax = info.recycle;
    cfg[TPS_RECYCLE].pmax = 1;
}

bool CommonServer::init(const ServerCfgBaseInfo& info)
{
    TPConfig cfg[TPS_MAX] = {{0,0,0,},};
    initCfg(info, cfg);

    if (!TcpServer::init(cfg))
        return false;

    if (!info.port)
    {
        // TODO: log
        return false;
    }

    setPort(info.port);
    setExtPort(info.extport);

    setIp(info.ip.c_str());
    setExtIp(info.extip.c_str());

    if (info.id)
        setId(info.id);
    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

