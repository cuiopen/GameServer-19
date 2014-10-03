
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/MemPool.h"
using namespace buf;

void test1()
{
    MemPool mp;
    void* p = mp.alloc(100);
    mp.printInfo();
    p = mp.alloc(100);
    mp.printInfo();
    mp.free(p);
    mp.printInfo();
    void* p1 = mp.alloc(100);
    mp.printInfo();
    void* p2 = mp.alloc(100);
    mp.printInfo();
    void* p3 = mp.alloc(100);
    mp.printInfo();
    void* p4 = mp.alloc(100);
    mp.printInfo();
    void* p5 = mp.alloc(256);
    mp.printInfo();
    void* p6 = mp.alloc(257);
    mp.printInfo();
    mp.free(p1);
    mp.printInfo();
    mp.free(p2);
    mp.printInfo();
    mp.free(p3);
    mp.printInfo();
    mp.free(p4);
    mp.printInfo();
}

int main(int argc, char* argv[])
{
    test1();
	return 0;
}
