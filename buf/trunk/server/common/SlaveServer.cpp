
#include "SlaveServer.h"
#include "MasterClient.h"
#include "Cfg.h"
#include "Packet.h"
#include "core/Logger.h"

namespace buf
{

SlaveServer::SlaveServer(const char* name, int type, int id = 0)
    : CommonServer(name, type, id), _master(0)
{
}

SlaveServer::~SlaveServer()
{
    if (_master)
        _master->join();
    DELETE(_master);
}

void SlaveServer::final()
{
    if (_master)
        _master->final();
    CommonServer::final(); // XXX: TcpServer::final()
}

bool SlaveServer::recvCfg(ServerCfgBaseInfo& cfg)
{
    sLog.trace("Waiting for reciving Server Configuration from MasterServer.");

    Packet info;
    int ret = 0;
    if ((ret = _master->sock()->recvPack(info.wbuf(), info.wsize(), true)) < 0)
    {
        _master->final();
        return false;
    }
    info.wflip(ret);
    cfg << info;
    return true;
}

bool SlaveServer::init()
{
    Table master = gCfg.get<Table>("master");
    const char* ip = master.get<const char*>("ip");
    if (!ip)
    {
        sLog.error("Have no master.ip in configuration.\n");
        return false;
    }
    port_t port = master.get<port_t>("port");
    if (!port)
    {
        sLog.error("Have no master.port in configuration.\n");
        return false;
    }

    _master = BUFNEW MasterClient("SlaveServer->MasterClient", ip, port, getType());
    if (!_master)
        return false;

    if (!reinterpret_cast<MasterClient*>(_master)->connect2("MasterServer"))
        return false;

    ServerCfgBaseInfo cfg;
    if (!recvCfg(cfg))
    {
        sLog.append(" [FAILED].\n");
        return false;
    }
    sLog.append(" [SUCC].\n");

    if (cfg.id > 1)
    {
        char sname[1024] = {0};
        SNPRINTF(sname, sizeof(sname), "%s-%u", sLog.getName(), cfg.id);
        sLog.setName(sname);
        sLog.setLoggerDir(sLog.getLoggerDir());

        char gname[1024] = {0};
        SNPRINTF(gname, sizeof(gname), "%s-%u", sLog.getName(), cfg.id);
        sLog.setName(gname);
        sLog.setLoggerDir(sLog.getLoggerDir());
    }

    if (!CommonServer::init(cfg))
    {
        sLog.error("%s: CommonServer::init\n", __PRETTY_FUNCTION__);
        return false;
    }

    if (!bind())
    {
        sLog.error("%s: bind()\n", __PRETTY_FUNCTION__);
        return false;
    }

    sLog.trace("%s: OK\n", __PRETTY_FUNCTION__);

    _master->start();
    while (!_master->isAlive())
        Thread::sleep(1);

    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

