
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include <signal.h>
#include "core/TcpServer.h"
using namespace buf;

TcpServer* pts = NULL;

void handler(int sig)
{
    (void)sig;
    if (pts)
        pts->final();
}

void test()
{
    struct sigaction s;
    s.sa_handler = handler;

    sigaction(SIGINT, &s, NULL);

    TcpServer ts("yangyoufa");
    pts = &ts;
    if (ts.init() && ts.bind("", 7777)) {
        ts.start();
        ts.join();
    }
    printf("over\n");
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    extern void alloc_leak();
    alloc_leak();
	return 0;
}
