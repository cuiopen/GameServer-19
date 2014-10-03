
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Bit.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Bit<31> bits;

    bits.set(0, 1);
    bits.set(1, 1);
    bits.set(1, 0);

    bits.set(9, 1);
    bits.set(7, 1);
    bits.set(20, 1);

	return 0;
}
