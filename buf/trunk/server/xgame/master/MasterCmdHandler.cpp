
#include "MasterCmdHandler.h"
#include "MasterProcessor.h"

namespace buf
{

#define TP(x) static_cast<MasterProcessor*>(tp)

int ServerReg(TcpProcess* tp, const void* data, size_t len)
{
    printf("%s: %s\n", __PRETTY_FUNCTION__, (const char*)data);
    TP(tp)->sendPack(data, len);
    return 0;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

