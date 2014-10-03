
#ifndef CMDHANDLER_H_
#define CMDHANDLER_H_

#include "core/Config.h"
#include "core/TcpProcess.h"

// XXX: must packed by 1
#pragma pack(push, 1)

namespace buf
{

int CmdNull(TcpProcess* tp, const void* body, size_t len);

struct Body
{
    int a;
    char name[32];
};

int CmdNull1(TcpProcess* tp, const Body& body);

} // namespace buf

#pragma pack(pop)

#endif // CMDHANDLER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

