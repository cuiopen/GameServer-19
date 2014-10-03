
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
}

#include "LoginServer.h"
#include "LoginTimer.h"
#include "core/Logger.h"
#include "common/Cfg.h"
#include "common/Servers.h"
#include "script/Script.h"

#define SERVER_SINGLE 1

using namespace buf;

TcpServer* pts = NULL;
LoginTimer* pmt = NULL;

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

    Table login = gCfg.get<Table>("login");
    if (!login.validate())
    {
        sLog.error("There is no [login] configuration.\n");
        return 0;
    }

#if SERVER_SINGLE == 0
    Table r = login.get<Table>(1);
    if (!r.validate())
    {   
        sLog.error("There is no login[1] configuration.\n");
        return 0;
    }
#endif

#if SERVER_SINGLE == 0
    const char* servername = r.get<const char*>("server_name");
#else
    const char* servername = login.get<const char*>("server_name");
#endif
    if (!servername)
        servername = "LoginServer";

    LoginServer ls(servername);
    pts = &ls;

#if SERVER_SINGLE == 0
    if (!CommonServer::getLogCfg(r, &ls))
#else
    if (!CommonServer::getLogCfg(login, &ls))
#endif
        sLog.warn("Logger configuration error.\n");
    
    sLog.trace("starting ... [OK]\n");

    if (ls.init())
    {
#if SERVER_SINGLE == 0
        if (ls.getId() != 1)
        {   
            Table r = login.get<Table>(ls.getId());
            if (!r.validate())
            {
                sLog.error("There is no login[%u] configuration.\n", ls.getId());
                return 0;
            }

            sLog.trace("Changing to configuration login[%u].\n", ls.getId());
            if (CommonServer::getLogCfg(r, &ls))
            {
                servername = r.get<const char*>("server_name");
                if (!servername)
                    servername = "LoginServer";

                ls.setName(servername);
                sLog.info("Change to configuration login[%u]. [OK]\n", ls.getId());
            }
        }
#endif

        ls.start();

        LoginTimer mt;
        pmt = &mt;
        mt.init();
        mt.start();

        mt.join();
        ls.join();
    }

    extern void alloc_leak();
    alloc_leak();
	return 0;
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

