
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
}

#include "RecordServer.h"
#include "RecordTimer.h"
#include "core/Logger.h"
#include "common/Cfg.h"
#include "common/Servers.h"
#include "script/Script.h"

#define SERVER_SINGLE 0

using namespace buf;

TcpServer* pts = NULL;
RecordTimer* pmt = NULL;

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

    Table record = gCfg.get<Table>("record");
    if (!record.validate())
    {
        sLog.error("There is no [record] configuration.\n");
        return 0;
    }

#if SERVER_SINGLE == 0
    Table r = record.get<Table>(1); // XXX: default
    if (!r.validate())
    {
        sLog.error("There is no record[1] configuration.\n");
        return 0;
    }
#endif

#if SERVER_SINGLE == 0
    const char* servername = r.get<const char*>("server_name");
#else
    const char* servername = game.get<const char*>("server_name");
#endif
    if (!servername)
        servername = "RecordServer";

    RecordServer rs(servername);
    pts = &rs;
    
#if SERVER_SINGLE == 0
    if (!CommonServer::getLogCfg(r, &rs))
#else
    if (!CommonServer::getLogCfg(record, &rs))
#endif
        sLog.warn("Logger configuration error.\n");

    sLog.trace("starting ... [OK]\n");

    if (rs.init())
    {
#if SERVER_SINGLE == 0
        if (rs.getId() != 1)
        {
            Table r = record.get<Table>(rs.getId());
            if (!r.validate())
            {
                sLog.error("There is no record[%u] configuration.\n", rs.getId());
                return 0;
            }

            sLog.trace("Changing to configuration record[%u].\n", rs.getId());
            if (CommonServer::getLogCfg(r, &rs))
            {
                servername = r.get<const char*>("server_name");
                if (!servername)
                    servername = "RecordServer";

                rs.setName(servername);
                sLog.info("Change to configuration record[%u]. [OK]\n", rs.getId());
            }
        }
#endif

        rs.start();

        RecordTimer mt;
        pmt = &mt;
        mt.init();
        mt.start();

        mt.join();
        rs.join();
    }

    extern void alloc_leak();
    alloc_leak();
	return 0;
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

