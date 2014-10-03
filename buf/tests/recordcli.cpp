
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/RecordClient.h"
#include "common/Servers.h"
#include "common/BufferClient.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    RecordClient<BufferClient> rc("Gateway-RecordServer", "172.16.219.128", 10000, SERVER_LOGIN);

    rc.setReconnect(true);
    if (rc.connect2())
    {
        rc.start();
    }

    rc.join();
	return 0;
}
