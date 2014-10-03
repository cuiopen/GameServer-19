
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Logger.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Logger* logger = BUFNEW Logger("yangyoufa");
    if (!logger)
        return -1;
#if 0
    //if (!logger->init("~/log"))
    //    return -1;
    while (true) {
        logger->setColored(true);
        logger->debug("hello\n");
        logger->trace("hello\n");
        logger->info("hello\n");
        logger->warn("hello\n");
        //logger->setColored(false);
        //logger->setLoggerDir("/tmp");
        logger->error("hello\n");
        logger->assert("hello\n");
        logger->fatal("hello\n");
        usleep(500*1000);
    }   
#endif
    DELETE(logger);

    //sLog.init("~/global");
    sLog.debug("OK1 - %s", "yangyoufa\n");
    sLog.info("OK1 - %s", "yangyoufa\n");
    LOG_DEBUG("OOOK1\n");
    sLog.info("OK2 - %s", "yangyoufa\n");
    sLog.debug("OK2 - %s", "yangyoufa\n");

    return 0;
}
