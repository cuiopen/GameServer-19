
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
}

#include "MasterServer.h"
#include "MasterTimer.h"
#include "core/Logger.h"
#include "common/Cfg.h"
#include "common/Servers.h"
#include "script/Script.h"

using namespace buf;

TcpServer* pts = NULL;
MasterTimer* pmt = NULL;

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

    Table master = gCfg.get<Table>("master");
    const char* servername = master.get<const char*>("server_name");
    if (!servername)
        servername = "MasterServer";

    MasterServer ms(servername);
    pts = &ms;

    if (!CommonServer::getLogCfg(master, &ms))
        sLog.warn("Logger configuration error.\n");

    sLog.trace("starting ... [OK]\n");

    if (!ms.init())
        return -1;

    if (ms.bind())
    {
        ms.start();

        MasterTimer mt;
        pmt = &mt;
        mt.init();
        mt.start();

        mt.join();
        ms.join();
    }
    else
    {
        sLog.error("Bind to %s:%u error.\n", ms.getIp(), ms.getPort());
    }

    extern void alloc_leak();
    alloc_leak();
	return 0;
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

