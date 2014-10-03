
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/New.h"
#include "core/Config.h"


#pragma pack(1)
struct A
{
#if 1
    A() {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
#endif
#if 1
    ~A() {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
#endif
};
#pragma pack()

void test(void)
{
    //char* buf = BUFNEW char[32];
    //DELETEV(buf);

    //A* pa = BUFNEW A();

    printf("sizeof(A): %"SZT_FMT"\n", sizeof(A));
    A* paa = BUFNEW A[2];

    //delete [] paa;
    DELETEV(paa);

    //A* p = BUFNEW A();
    //delete p;

}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    extern void alloc_leak();
    alloc_leak();
	return 0;
}
