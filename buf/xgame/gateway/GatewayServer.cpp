
#include "GatewayServer.h"
#include "GatewayProcessor.h"
#include "GameClientMgr.h"
#include "common/Cfg.h"

namespace buf
{

GatewayServer::GatewayServer(const char* name) : SlaveServer(name, SERVER_GATEWAY, 0)
{}

GatewayServer::~GatewayServer()
{}

bool GatewayServer::init()
{
    if (!SlaveServer::init())
        return false;
    if (!GCM.init())
        return false;
    if (!initGameClients())
        return false;
    return true;
}

bool GatewayServer::initGameClients()
{
    Table game = gCfg.get<Table>("game");
    if (!game.validate())
    {
        return false;
    }

    int size = game.size();
    if (!size)
    {
        return false;
    }

    for (int i = 1; i <= size; ++i)
    {
        Table g = game.get<Table>(i);
        if (!g.validate())
        {
            return false;
        }

        const char* ip = g.get<const char*>("ip");
        if (!ip)
        {
            return false;
        }

        port_t port = g.get<port_t>("port");
        if (!port)
        {
            return false;
        }

        GameClient* gc = BUFNEW GameClient(ip, port, SERVER_GATEWAY);
        if (!gc)
        {
            return false;
        }

        char name[1024] = {0};
        SNPRINTF(name, sizeof(name), "GameServer-%d", i);
        if (!gc->connect2(name))
        {
            return false;
        }

        if (!GCM.add(gc))
        {
            return false;
        }
    }

    return true;
}

TcpProcessor* GatewayServer::createProcessor(socket_t s, sockaddrin_t* sa) 
{
    if (s < 0 || !sa)
        return 0;
    return new GatewayProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

