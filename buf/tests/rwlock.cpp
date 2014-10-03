
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/RWLock.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    RWLock rw;
    ScopeRWLock srw(rw);
    ScopeRDLock sdw(rw);

	return 0;
}
