
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/SpinLock.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    SpinLock lock;

    lock.lock();
    lock.lock();

    lock.unlock();
    lock.unlock();

	return 0;
}
