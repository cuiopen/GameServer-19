
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/MCached.h"
using namespace buf;

int main(int argc, char* argv[])
{
    MCached m;
    if (m.init("localhost"))
        fprintf(stderr, "OK\n");
    else
        fprintf(stderr, "ERROR\n");

    m.set("yangyoufa", "100", 3);

    char value[1024] = {0};
    printf("get: %s\n", m.get("yangyoufa", value, sizeof(value)));

    sleep(2);
    memset(value, 0x00, sizeof(value));
    printf("get: %s\n", m.get("yangyoufa", value, sizeof(value)));

    sleep(2);
    memset(value, 0x00, sizeof(value));
    printf("get: %s\n", m.get("yangyoufa", value, sizeof(value)));

	return 0;
}
