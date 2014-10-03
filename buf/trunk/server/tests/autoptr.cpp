
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/AutoPtr.h"
#include "core/New.h"

#include <string>

using namespace buf;

void test(void)
{
    AutoArrPtr<char> ptr(BUFNEW char[100]);
    AutoPtr<std::string> ptrs(BUFNEW std::string());

    // diable
    //AutoPtr<std::string> ptrs1 = BUFNEW std::string();
    // AutoPtr<std::string> ptrs2(ptrs1);

}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    extern void alloc_leak();
    alloc_leak();
	return 0;
}

