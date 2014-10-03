
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/GMSystem.h"
using namespace buf;

static void test(const Tokenizer& args)
{
    printf("%s\n", __PRETTY_FUNCTION__);
}

static void help(const Tokenizer& args)
{
    printf("%s\n", __PRETTY_FUNCTION__);
}

static const char* getDesc(int id)
{
    return "test";
}

static GMHandler KGMHANDLERS[] =
{
    {"help", &help, GMT_ALL, "help"},
    {"test", &test, GMT_ALL, getDesc(0)},
    {0,0,0,0}
};

int main(int argc, char* argv[])
{
    GMParse(KGMHANDLERS, sizeof(KGMHANDLERS)/sizeof(GMHandler), "test");
    return 0;
}
