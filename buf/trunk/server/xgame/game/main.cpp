
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
}

#include "GameServer.h"
#include "GameTimer.h"
#include "core/Logger.h"
#include "common/Cfg.h"
#include "common/Servers.h"
#include "script/Script.h"

#define SERVER_SINGLE 0

using namespace buf;

TcpServer* pts = NULL;
GameTimer* pmt = NULL;

void handler(int sig)
{
    (void)sig;
    if (pmt)
        pmt->final();
    if (pts)
        pts->final();
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    struct sigaction s;
    s.sa_handler = handler;

    sigaction(SIGINT, &s, NULL);

    if (!gCfg.load(Cfg::getCfgFile()))
        return 0;

    Table game = gCfg.get<Table>("game");
    if (!game.validate())
    {
        sLog.error("There is no [game] configuration.\n");
        return 0;
    }

#if SERVER_SINGLE == 0
    Table r = game.get<Table>(1);
    if (!r.validate())
    {   
        sLog.error("There is no game[1] configuration.\n");
        return 0;
    }
#endif

#if SERVER_SINGLE == 0
    const char* servername = r.get<const char*>("server_name");
#else
    const char* servername = game.get<const char*>("server_name");
#endif
    if (!servername)
        servername = "GameServer";

    GameServer gs(servername);
    pts = &gs;

#if SERVER_SINGLE == 0
    if (!CommonServer::getLogCfg(r, &gs))
#else
    if (!CommonServer::getLogCfg(game, &gs))
#endif
        sLog.warn("Logger configuration error.\n");
    
    sLog.trace("starting ... [OK]\n");

    if (gs.init())
    {
#if SERVER_SINGLE == 0
        if (gs.getId() != 1)
        {   
            Table r = game.get<Table>(gs.getId());
            if (!r.validate())
            {
                sLog.error("There is no game[%u] configuration.\n", gs.getId());
                return 0;
            }

            sLog.trace("Changing to configuration game[%u].\n", gs.getId());
            if (CommonServer::getLogCfg(r, &gs))
            {
                servername = r.get<const char*>("server_name");
                if (!servername)
                    servername = "GameServer";

                gs.setName(servername);
                sLog.info("Change to configuration game[%u]. [OK]\n", gs.getId());
            }
        }
#endif

        gs.start();

        GameTimer mt;
        pmt = &mt;
        mt.init();
        mt.start();

        mt.join();
        gs.join();
    }

    extern void alloc_leak();
    alloc_leak();
	return 0;
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

