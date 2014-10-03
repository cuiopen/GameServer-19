
#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include "common/SlaveServer.h"

namespace buf
{

class GameServer : public SlaveServer
{
public:
    GameServer(const char* name);
    virtual ~GameServer();

    virtual bool init();

    inline bool ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info)
    {
        return true;
    }

    inline bool sync(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool process(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool recycle(s32_t type, s32_t id)
    {
        return true;
    }

    virtual TcpProcessor* createProcessor(socket_t s, sockaddrin_t* sa);

private:
};

} // namespace buf

#endif // GAMESERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

