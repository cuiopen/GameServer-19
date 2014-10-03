
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Config.h"

int main(int argc, char* argv[])
{

    unsigned int a = 100;
    int b = -10;

    a += b;
    printf("%u\n", a);

    DEBUG("xxxxx %s\n", "sdfsdf");
	return 0;
}
