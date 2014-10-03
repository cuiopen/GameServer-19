
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/TcpConnectionPool.h"
#include "core/TcpConnection.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    TcpConnectionPool tcp;
    tcp.init();

    static int i = 0;
    TcpConnection* tc = new TcpConnection("", 7777);
    if (tc) {
        char buf[100] = {0};
        if (tc->connect()) {
            tcp.add(tc);

            snprintf(buf, 100, "hello - %d", ++i);
            tc->sendPack(buf, strlen(buf)); // ack

            while (true) {
                sleep(1);
            }
            tcp.join();
        }
    }

	return 0;
}
