
#include "GatewayProcessor.h"
#include "GatewayProcessorMgr.h"
#include "GatewayServer.h" // for GatewayServer
#include "GatewayCmd.h"
#include "common/Packet.h"

namespace buf
{

bool GatewayProcessor::uniqueAdd()
{
    return GatewayProcessorMgr::getInstance().add(this);
}

void GatewayProcessor::uniqueRemove()
{
    GatewayProcessorMgr::getInstance().remove(this);
}

int GatewayProcessor::ack(const void* pack, int len)
{
    if (len < (int)sizeof(s32_t))
        return -1;

    s32_t type = *(s32_t*)pack;
    sLog.trace("ACK with type: %d from: %s\n", type, sock()->getRemoteIp());

    // TODO: 安全检查
    setType(type);
    return 1;
}

int GatewayProcessor::sync()
{
    // TODO:
    return 1;
}

int GatewayProcessor::process(const void* pack, int len)
{
    if (!_gs)
        return -1;

    bool ret = MessageQueue::pushCmd(pack, len);
    if (!ret)
    {
        // XXX: warn
    }
    return 0;
}

int GatewayProcessor::recycle()
{
    //
    sLog.trace("Recycle %d from: %s\n", _type, sock()->getRemoteIp());
    return 1;
}

void GatewayProcessor::heartbeat()
{
}

void GatewayProcessor::onConnected()
{
}

void GatewayProcessor::onEnterProcess()
{
}

bool GatewayProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

