
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "common/MsgMgr.h"
using namespace buf;

int main(int argc, char* argv[])
{
    MsgMgr mm;
    mm.set(100, 1, 1, "hello");
    mm.set(100, "hello1");

    printf("%s\n", mm.get(100)->msg.c_str());

	return 0;
}
