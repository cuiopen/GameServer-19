
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
    const char* servername = login.get<const char*>("server_name");
    if (!servername)
        servername = "LoginServer";
    
    LoginServer ls(servername);
    pts = &ls;

    if (!CommonServer::getLogCfg(login, &ls))
        sLog.warn("Logger configuration error.\n");

    sLog.trace("starting ... [OK]\n");

    if (ls.init())
    {
        ls.start();

        LoginTimer lt;
        pmt = &lt;
        lt.init();
        lt.start();

        lt.join();
        ls.join();
    }

    extern void alloc_leak();
    alloc_leak();
	return 0;
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

