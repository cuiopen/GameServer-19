
#include "LoginProcessor.h"
#include "LoginProcessorMgr.h"
#include "LoginServer.h" // for LoginServer
#include "LoginCmd.h"
#include "common/Packet.h"

namespace buf
{

bool LoginProcessor::uniqueAdd()
{
    return LoginProcessorMgr::getInstance().add(this);
}

void LoginProcessor::uniqueRemove()
{
    LoginProcessorMgr::getInstance().remove(this);
}

int LoginProcessor::ack(const void* pack, int len)
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

int LoginProcessor::sync()
{
    if (getType() == SERVER_NONE)
        return 1;
    return 1;
}

int LoginProcessor::process(const void* pack, int len)
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

int LoginProcessor::recycle()
{
    if (_recycle || getType() == SERVER_NONE)
        return 1;
    printf("%s : %s\n", __PRETTY_FUNCTION__, sock()->getRemoteIp());
    return 1;
}

void LoginProcessor::heartbeat()
{
}

void LoginProcessor::onConnected()
{
}

void LoginProcessor::onEnterProcess()
{
}

bool LoginProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

