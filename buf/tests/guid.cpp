
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "common/GUID.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    printf("sizeof(GUID): %lu\n", sizeof(GUID));
    gGUID.set(GUID(GUID_t(0)));

    GUID guid = gGUID.get(GUID_ITEM);
    printf("%lu\n", guid.getCounter());
    guid = gGUID.get(GUID_ITEM);
    printf("%lu\n", guid.getCounter());
    guid = gGUID.get(GUID_ITEM);
    printf("%lu\n", guid.getCounter());
    guid = gGUID.get(GUID_ITEM);
    printf("%lu\n", guid.getCounter());
    guid = gGUID.get(GUID_ITEM);
    printf("%lu\n", guid.getCounter());

    GUID guid1 = gGUID.get(GUID_PLAYER);
    if (guid1 < guid)
        printf("%lx < %lx\n", (u64_t)guid1, (u64_t)guid);
    else
        printf("%lx >= %lx\n", (u64_t)guid1, (u64_t)guid);


	return 0;
}
