
#include "MasterClient.h"
#include "core/System.h"
#include "core/Logger.h"

namespace buf
{

int MasterClient::ack(const void* pack, int len)
{
    UNUSE(pack); UNUSE(len);
    return 1;
}

int MasterClient::process(const void* pack, int len)
{
    UNUSE(pack); UNUSE(len);
    return 1;
}

void MasterClient::heartbeat()
{
}

void MasterClient::onConnected()
{
    if (connected())
    {
        sLog.trace("ACK(%u) to Server: %s:%u.", getType(), getIp(), getPort());
        s32_t type = getType();
        if (forceSendPack(&type, sizeof(type)) < 0)
        {
            sLog.append(" [FAILED].\n");
            return;
        }
        sLog.append(" [SUCC].\n");
        return;
    }
}

void MasterClient::onDisconnected()
{
    fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
}

void MasterClient::onEnterProcess()
{
    return;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

