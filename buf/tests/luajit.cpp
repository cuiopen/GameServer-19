
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "script/Script.h"
using namespace buf;

void test1()
{
    Script s;
    //s.doString("print(__s64);print(__u64)");
    s.doString("ffi=require(\"ffi\"); if ffi ~= nil then print('ffi is OK') else print('ffi is NOT OK') end");
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    test1();
	return 0;
}
