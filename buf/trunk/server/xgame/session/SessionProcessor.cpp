
#include "SessionProcessor.h"
#include "SessionProcessorMgr.h"
#include "SessionServer.h" // for SessionServer
#include "SessionCmd.h"
#include "common/Packet.h"

namespace buf
{

bool SessionProcessor::uniqueAdd()
{
    return SessionProcessorMgr::getInstance().add(this);
}

void SessionProcessor::uniqueRemove()
{
    SessionProcessorMgr::getInstance().remove(this);
}

int SessionProcessor::ack(const void* pack, int len)
{
    if (len < (int)sizeof(s32_t))
        return -1;

    s32_t type = *(s32_t*)pack;
    sLog.trace("ACK with type: %d from: %s\n", type, sock()->getRemoteIp());

    // XXX: GM - must check ip
    if (type == SERVER_NONE)
    {
        if (_ls && _ls->ipMask(sock()->getRemoteIp()))
        {
            _recycle = false;
            return 1;
        }

        goto _err;
    }

    // XXX: 客户端
    _recycle = false;
    return 1;

_err:
    sLog.error("ACK with type: %d from: %s\n", type, sock()->getRemoteIp());
    return -1;
}

int SessionProcessor::sync()
{
    if (getType() == SERVER_NONE)
        return 1;
    return 1;
}

int SessionProcessor::process(const void* pack, int len)
{
    if (!_ls)
        return -1;

    bool ret = MessageQueue::pushCmd(pack, len);
    if (!ret)
    {
        // XXX: warn
    }
    return 0;
}

int SessionProcessor::recycle()
{
    if (_recycle || getType() == SERVER_NONE)
        return 1;
    printf("%s : %s\n", __PRETTY_FUNCTION__, sock()->getRemoteIp());
    return 1;
}

void SessionProcessor::heartbeat()
{
}

void SessionProcessor::onConnected()
{
}

void SessionProcessor::onEnterProcess()
{
}

bool SessionProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

