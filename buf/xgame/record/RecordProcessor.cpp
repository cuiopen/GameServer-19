
#include "RecordProcessor.h"
#include "RecordProcessorMgr.h"
#include "RecordServer.h" // for RecordServer
#include "RecordCmd.h"
#include "common/Packet.h"

namespace buf
{

bool RecordProcessor::uniqueAdd()
{
    return RecordProcessorMgr::getInstance().add(this);
}

void RecordProcessor::uniqueRemove()
{
    RecordProcessorMgr::getInstance().remove(this);
}

int RecordProcessor::ack(const void* pack, int len)
{
    if (len < (int)sizeof(s32_t))
        return -1;

    s32_t type = *(s32_t*)pack;
    sLog.trace("ACK with type: %d from: %s\n", type, sock()->getRemoteIp());

    // TODO: 安全检查
    setType(type);
    return 1;
}

int RecordProcessor::sync()
{
    // TODO:
    return 1;
}

int RecordProcessor::process(const void* pack, int len)
{
    if (!_rs)
        return -1;

    bool ret = MessageQueue::pushCmd(pack, len);
    if (!ret)
    {
        // XXX: warn
    }
    return 0;
}

int RecordProcessor::recycle()
{
    // TODO:
    sLog.trace("Recycle %d from: %s\n", _type, sock()->getRemoteIp());
    return 1;
}

void RecordProcessor::heartbeat()
{
}

void RecordProcessor::onConnected()
{
}

void RecordProcessor::onEnterProcess()
{
}

bool RecordProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

