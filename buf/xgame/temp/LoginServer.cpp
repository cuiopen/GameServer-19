
#include "LoginServer.h"
#include "LoginProcessor.h"
#include "common/Cfg.h"

namespace buf
{

LoginServer::LoginServer(const char* name) : SlaveServer(name, SERVER_LOGIN, 0)
{}

LoginServer::~LoginServer()
{}

bool LoginServer::init()
{
    if (!SlaveServer::init())
        return false;

    // XXX: just LoginServer and MasterServer need this
    Table login = gCfg.get<Table>(getServerTypeString(getType()));
    Table mask = login.get<Table>("mask");
    getIpMask(mask);

    return true;
}

bool LoginServer::ipMask(const char* ip) 
{
    if (!ip)
        return false;
    return gCfg.isIPAllowed(ip);
}

TcpProcessor* LoginServer::createProcessor(socket_t s, sockaddrin_t* sa) 
{
    if (s < 0 || !sa)
        return 0;
    return new LoginProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

