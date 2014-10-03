
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "common/SlaveServer.h"
#include "common/Servers.h"
#include "common/Cfg.h"

using namespace buf;

int main(int argc, char* argv[])
{
    gCfg.load("config.lua");

    //SlaveServer ss("SlaveServer", SERVER_RECORD, 0);
    SlaveServer ss("SlaveServer", SERVER_LOGIN, 0);
    if (ss.init())
    {
        ss.start();
        ss.join();
    }
	return 0;
}
