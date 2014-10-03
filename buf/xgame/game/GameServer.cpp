
#include "GameServer.h"
#include "GameProcessor.h"
#include "common/Cfg.h"

namespace buf
{

GameServer::GameServer(const char* name) : SlaveServer(name, SERVER_GAME, 0)
{}

GameServer::~GameServer()
{}

bool GameServer::init()
{
    if (!SlaveServer::init())
        return false;
    return true;
}

TcpProcessor* GameServer::createProcessor(socket_t s, sockaddrin_t* sa) 
{
    if (s < 0 || !sa)
        return 0;
    return new GameProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

