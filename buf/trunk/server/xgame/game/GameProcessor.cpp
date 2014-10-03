
#include "GameProcessor.h"
#include "GameProcessorMgr.h"
#include "GameServer.h" // for GameServer
#include "GameCmd.h"
#include "common/Packet.h"

namespace buf
{

bool GameProcessor::uniqueAdd()
{
    return GameProcessorMgr::getInstance().add(this);
}

void GameProcessor::uniqueRemove()
{
    GameProcessorMgr::getInstance().remove(this);
}

int GameProcessor::ack(const void* pack, int len)
{
    if (len < (int)sizeof(s32_t))
        return -1;

    s32_t type = *(s32_t*)pack;
    sLog.trace("ACK with type: %d from: %s\n", type, sock()->getRemoteIp());

    type = _gs->getType();
    sLog.trace("Send ACK to %s", sock()->getRemoteIp());
    if (sendPack(&type, sizeof(type)) < 0)
    {
        sLog.append("FAILED.\n");
        return -1;
    }
    sLog.append("SUCC.\n");

    // TODO: 安全检查
    setType(type);
    return 1;
}

int GameProcessor::sync()
{
    // TODO:
    return 1;
}

int GameProcessor::process(const void* pack, int len)
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

int GameProcessor::recycle()
{
    //
    sLog.trace("Recycle %d from: %s\n", _type, sock()->getRemoteIp());
    return 1;
}

void GameProcessor::heartbeat()
{
}

void GameProcessor::onConnected()
{
}

void GameProcessor::onEnterProcess()
{
}

bool GameProcessor::cmdProcessor(const void* pack, unsigned int len)
{
    return CmdHandlerDoProcess(this, pack, len);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

