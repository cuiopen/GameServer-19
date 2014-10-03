
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
}

#include "core/RTime.h"
#include "core/Logger.h"
using namespace buf;

void test()
{
    TimeFilm fl("Test time: ", &sLog); 

    sleep(1);
    usleep(400000);
    Time t;
    printf("%d/%d/%d\n", t.year(), t.month(), t.day());
    printf("%d:%d:%d\n", t.hour(), t.min(), t.sec());
    printf("secs:%lu\n", t.secs());

    printf("time:%s\n", t.toString());

    RTime rt; 
    printf("sces: %ld\n", rt.secs());
    printf("msces: %ld\n", rt.msecs());
    printf("msce: %ld\n", rt.msec());

    RTime rt1 = rt;
    rt = rt1;

    printf("tick: %lu\n", RTime::tick());
    printf("tick: %lu\n", RTime::tick());
    Time n;
    Time::theDay(1);
    Time::theWeek(1);
    Time::theMonth(1);
    Time::theYear(1);
    timediff_t usecs = 0;
    printf("usecs: %lu, tick: %lu\n", usecs, RTime::tick(0, &usecs));
}

void test1()
{
    TimeInterval ti(5000);
    RTime now;
    if (ti(now))
        printf("5 seconds 1\n");
    sleep(2);
    now.now();
    if (ti(now))
        printf("5 seconds 2\n");
    sleep(3);
    now.now();
    if (ti(now))
        printf("5 seconds 3\n");
}

void test2()
{
    TimeFilm fl("YYF", &sLog);
    //TimeFilm fl("YYF");
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    test1();
    test2();
    return 0;
}
