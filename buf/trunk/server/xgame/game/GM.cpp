
#include "GM.h"

namespace buf
{

void test(const Tokenizer& params)
{}

// TODO: 在这之上增加函数并在GMCMD里注册

static GMHandler GMCMD[] =
{
    {"test", &test, GMT_ALL, "test"},
};

// XXX: readonly
void GM(const char* cmd, int mode)
{
    GMParse(GMCMD, sizeof(GMCMD)/sizeof(GMHandler), cmd, mode);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

