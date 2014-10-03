
#ifndef CMD_H_
#define CMD_H_

#include "core/Config.h"
#include "core/Assertion.h"
#include "CmdHelper.h"
#include "CmdHandler.h"

namespace buf
{

// XXX: a template of command
// XXX: 这里不要注册任何指令，只是一个模板。
//      只能在各自的服务器相关的XXXCmd.h文件里注册。

enum CMD_TYPE
{
    CMD_NULL = 0,
    CMD_MAX
};

enum CMD_CMD
{
    CMD_CMD_NULL = 0,
    //CMD_CMD_NULL1 = 1,
    CMD_CMD_MAX
};

namespace REQ
{
    CMDDEF(CMDNULL, CMD_NULL, CMD_CMD_NULL, CmdNull);
    //CMDDEF(CMDNULL1, CMD_NULL, CMD_CMD_NULL1, CmdNull1);
} // REQ

namespace REP
{
} // REP

} // namespace buf

#endif // CMD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

