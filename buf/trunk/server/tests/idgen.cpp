
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Random.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Random rnd;

    for (int i = 0; i < 100000; ++i)
    {
        printf("1-%06u-%06u\n", rnd(100000), i);
    }

    for (int i = 0; i < 400000; ++i)
    {
        printf("2-%06u-%06u\n", rnd(400000), i);
    }

	return 0;
}
