
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <core/ThreadGroup.h>
#include <iostream>
#include <algorithm>
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    ThreadGroup tgroup("BSC");
    char name[128] = {0};
    Thread* thread = 0;
    for (int i=0; i<10; ++i) {
        SNPRINTF(name, sizeof(name), "Thread-%d", i); 
        thread = new Thread(name);
        if (thread) {
            if (tgroup.put(thread)) {
                fprintf(stderr, "%s.\n", name);
            } else {
                fprintf(stderr, "put thread error.\n");
                delete thread;
                break;
            }   
            MEMSET(name, 0x00, sizeof(name));
        } else 
            break;
    }

#ifdef CXX0X
    std::for_each(tgroup.begin(), tgroup.end(), [](Thread* t) {
            if (t)
                fprintf(stderr, "start: %s\n", t->getName());
                t->start();
            });
#else
    tgroup.startAll();
#endif

#if 0
    std::for_each(tgroup.begin(), tgroup.end(), [](Thread* t) {
            if (t)
                t->join();
            });
#endif
    tgroup.joinAll();

    return 0;
}
