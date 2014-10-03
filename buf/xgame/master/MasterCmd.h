
#ifndef MASTERCMD_H_
#define MASTERCMD_H_

#include "common/Cmd.h"
#include "MasterCmdHandler.h"

enum MASTER_CMD_TYPE
{
    MASTER_CMD_REG = 0,
    MASTER_CMD_MAX
};

enum MASTER_CMD_CMD
{
};

namespace buf
{
    CMDDEF(CMDREG, MASTER_CMD_REG, 1, ServerReg);

} // namespace buf

#endif // MASTERCMD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

