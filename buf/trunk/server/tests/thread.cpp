
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Thread.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Thread thread;
    thread.start();

    Thread thread1("YYF");
    thread1.start();

    thread.join();
    thread1.join();

	return 0;
}
