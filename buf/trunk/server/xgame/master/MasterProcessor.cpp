
#include "MasterProcessor.h"
#include "MasterProcessorMgr.h"
#include "MasterServer.h" // for MasterServer
#include "MasterCmd.h"
#include "common/Packet.h"

namespace buf
{

bool MasterProcessor::uniqueAdd()
{
    return MasterProcessorMgr::getInstance().add(this);
}

void MasterProcessor::uniqueRemove()
{
    MasterProcessorMgr::getInstance().remove(this);
}

int MasterProcessor::ack(const void* pack, int len)
{
    if (len < (int)sizeof(s32_t))
        return -1;

    ServerCfgBaseInfo* info = 0;
    s32_t type = *(s32_t*)pack;
    sLog.trace("ACK with type: %d from: %s", type, sock()->getRemoteIp());

    // XXX: GM,或不需要获取配置的连接
    if (type == SERVER_NONE)
    {
        if (_ms && _ms->ipMask(sock()->getRemoteIp()))
        {
            setType(type);
            _recycle = false;
            sLog.append(" [OK]\n");
            return 1;
        }

        goto _err;
    }

    // XXX: 需要返回配置的连接
    if (_ms && _ms->ack(type, sock()->getRemoteIp(), info))
    {
        setType(type);
        setServerId(info->id);

        Packet bb;
        *info >> bb;
        if (sendPack(bb.rbuf(), bb.rsize()) < 0)
            goto _err;

        _recycle = false;
        sLog.append(" [OK]\n");
        return 1;
    }

_err:
    sLog.append(" [FAILD]\n");
    return -1;
}

int MasterProcessor::sync()
{
    if (getType() == SERVER_NONE)
        return 1;
    if (_ms && _ms->sync(getType(), getServerId()))
        return 1;
    return 0;
}

int MasterProcessor::process(const void* pack, int len)
{
    if (!_ms)
        return -1;

    if (getType() != SERVER_NONE && !_process)
    {
        if (!_ms->process(getType(), getServerId()))
            return -1;
        _process = true;
    }

    bool ret = MessageQueue::pushCmd(pack, len);
    if (!ret)
    {
        // XXX: warn
    }
    return 0;
}

int MasterProcessor::recycle()
{
    if (_recycle || getType() == SERVER_NONE)
        return 1;
    printf("%s : %s\n", __PRETTY_FUNCTION__, sock()->getRemoteIp());
    if (_ms && getType() && getServerId() && _ms->recycle(getType(), getServerId()))
        return 1;
    return 0;
}

void MasterProcessor::heartbeat()
{
    fprintf(stderr, "%s: (%d)\n", __PRETTY_FUNCTION__, getId());
}

void MasterProcessor::onConnected()
{
    fprintf(stderr, "%s: (%d)\n", __PRETTY_FUNCTION__, getId());
}

void MasterProcessor::onEnterProcess()
{
    fprintf(stderr, "%s: (%d)\n", __PRETTY_FUNCTION__, getId());
}

void MasterProcessor::onDisconnected()
{
    fprintf(stderr, "%s: (%d)\n", __PRETTY_FUNCTION__, getId());
}

bool MasterProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

