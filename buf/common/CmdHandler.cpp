
#include "CmdHandler.h"

namespace buf
{

int CmdNull(TcpProcess* tp, const void* body, size_t len)
{
    UNUSE(body);
    UNUSE(len);
    UNUSE(tp);
    printf("%s: %s\n", __PRETTY_FUNCTION__, (const char*)body);
    return 0;
}

int CmdNull1(TcpProcess* tp, const Body& body)
{
    UNUSE(tp);
    UNUSE(body);
    printf("%s: %d, %s\n", __PRETTY_FUNCTION__, body.a, body.name);
    return 0;
}

}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

