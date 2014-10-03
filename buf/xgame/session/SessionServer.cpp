
#include "SessionServer.h"
#include "SessionProcessor.h"
#include "common/Cfg.h"

namespace buf
{

SessionServer::SessionServer(const char* name) : SlaveServer(name, SERVER_SESSION, 0)
{}

SessionServer::~SessionServer()
{}

bool SessionServer::init()
{
    if (!SlaveServer::init())
        return false;

    // XXX: just SessionServer and MasterServer need this
    Table login = gCfg.get<Table>(getServerTypeString(getType()));
    Table mask = login.get<Table>("mask");
    getIpMask(mask);

    return true;
}

bool SessionServer::ipMask(const char* ip) 
{
    if (!ip)
        return false;
    return gCfg.isIPAllowed(ip);
}

TcpProcessor* SessionServer::createProcessor(socket_t s, sockaddrin_t* sa) 
{
    if (s < 0 || !sa)
        return 0;
    return new SessionProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

