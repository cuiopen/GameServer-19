
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Mutex.h"
#include "core/RTime.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Mutex lock;
    lock.lock();
    lock.unlock();

    TimeFilm fl; 
    for (int i = 0; i < 30000; ++i)
        ScopeMutex slock(lock);

	return 0;
}
