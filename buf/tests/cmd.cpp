
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Logger.h"
#include "common/Cmd.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;

    const char* test = "xxxx fuck";

    struct C
    {
        CMD_t cmd;
        char words[32];
    } cmd;

#if 1
    cmd.cmd = REQ::CMDNULL;
    memcpy(cmd.words, test, strlen(test));
    CmdHandlerDoProcess(NULL, &cmd, sizeof(cmd));
#endif
    printf("%s\n", GetCmdName(cmd.cmd));

    struct B
    {
        CMD_t cmd;
        Body b;
    } cmd1;

#if 0
    cmd1.cmd = REQ::CMDNULL1;
    cmd1.b.a = 100;
    strcpy(cmd1.b.name, "KKK");
    CmdHandlerDoProcess(NULL, &cmd1, sizeof(cmd1));
#endif

	return 0;
}
