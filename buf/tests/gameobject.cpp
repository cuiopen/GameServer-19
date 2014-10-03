
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/GameObject.h"
using namespace buf;

int main(int argc, char* argv[])
{
#if 0
    GameObject go(100);

    go.setQwordValue(GO_GUID, 1000);
    go.setDwordValue(GO_TYPE, 9);

    printf("f:0 v:%lu\n", go.getQwordValue(GO_GUID));
    printf("f:2 v:%u\n", go.getDwordValue(GO_TYPE));
#endif
	return 0;
}
