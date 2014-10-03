
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Config.h"
#include "core/IDGenerator.h"
using namespace buf;

int test(void)
{
	return 0;
}


void test1(void)
{
    IDGenerator u(244);

    int pb = 0;
    unsigned char v = 0;
    for (int i=0; i<256; ++i) {
        v = u.get();
        if (v == u.max()) {
            printf("out of ids\n");
            break;
        }

        printf("id: %u ", v);
        if (pb++%5) {
            printf("put %u back\n", v);
            u.put(v);
            sleep(1);
        } else
            printf("\n");
    }
}

void test2(void)
{
    IDGenerator u(117);

    int pb = 0;
    unsigned char v = 0;
    for (int i=0; i<128; ++i) {
        v = u.get();
        if (v == u.max()) {
            printf("out of ids\n");
            break;
        }

        printf("id: %u ", v);
        if (pb++%5) {
            printf("put %u back\n", v);
            u.put(v);
            sleep(1);
        } else
            printf("\n");
    }
}

void test3(void)
{
    RandIDGenerator rkg;
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    test1();
    //test2();
    //IDGenerator<std::string> skg;
    //skg.get();
    //skg.put("");
    return 0;
}

