
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Socket.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    socket_t fd = 0;
    Socket s(fd, NULL);

    s.isZipOn();
	return 0;
}
