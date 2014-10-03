
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Atomic.h"
#include "core/ThreadGroup.h"

#include <algorithm>
using namespace buf;

void test1(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Atomic a(100);

    a++;
    ++a;
    --a;
    a--;
    a++;
    ++a;
    int ya = (int)a;
    printf("ya: %d\n", ya);
}

static Atomic cnt;

class CountThread : public Thread
{
public:
    CountThread(const char* name) : Thread(name) {}
    virtual void run()
    {
        while (true)
        {
            ++cnt;
            printf("cnt: %u\n", (unsigned int)cnt);
            sleep(1);
        }
    };

};

void test2(void)
{
    ThreadGroup tgroup("YYF", 100);
    char name[128] = {0};
    Thread* thread = 0;
    for (int i=0; i<3; ++i) {
        SNPRINTF(name, sizeof(name), "Thread-%d", i); 
        thread = new CountThread(name);
        if (thread) {
            if (tgroup.put(thread)) {
            } else {
                delete thread;
                break;
            }   
            MEMSET(name, 0x00, sizeof(name));
        } else 
            break;
    }


    tgroup.startAll();
    tgroup.joinAll();
}

int main(int argc, char* argv[])
{
    //test1(argc, argv);
    test2();
    return 0;
}

