
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#define _NDEBUG
#define USE_MEM_POOL
#include "core/MsgQueue.h"
#include "core/RTime.h"
using namespace buf;

void test1()
{
    TimeFilm tm("msg queue: ");
    MsgQueue<> mq;
    char buf[4096] = {0};
    size_t size = 128;
    int i = 1000000;
    while (--i)
    {
        mq.put(buf, size);
    }
    mq.printInfo();
    int j = 0;
    while (mq.get())
    {
        mq.flushToArray();
        ++j;
        mq.erase();
    }
    mq.printInfo();

    printf("msgs: %d\n", j);
}

int main(int argc, char* argv[])
{
    test1();
	return 0;
}
