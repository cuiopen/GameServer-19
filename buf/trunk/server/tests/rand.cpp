
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Random.h"
#include <unistd.h>

using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;

#if 0
    for (int i=0; i<100; ++i) {
        usleep(400);
        printf("rand : %u\n", rand_value());
    }
#endif


    Random rnd;
    for (int i=0; i<10; ++i) {
        printf("rand_between: 1 - 100 => %d\n", rnd(0,100));
    }

    for (int i=0; i<10; ++i) {
        printf("rand_between: 1 - 100 => %d\n", Random()(0,100));
    }
	return 0;
}
