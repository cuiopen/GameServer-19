
#ifndef GAMECLIENTMGR_H_
#define GAMECLIENTMGR_H_

#include "GameClient.h"
#include "core/Singleton.h"
#include "core/TcpConnectionPool.h"

namespace buf
{

static const struct TPConfig GCM_TPP_MAX[] = {
    {4, 4, "ACK"},
    {0, 0, "SYNC"},
    {4, 4, "PROCSSING"}, // XXX: 16 gameservers
    {1, 16, "RECYCLE"},
    {0,0,""}
};

class GameClientMgr : public Singleton<GameClientMgr>
{
public:
    GameClientMgr() {}
    ~GameClientMgr() {}

    bool init()
    {
        return _pool.init(GCM_TPP_MAX);
    }

    bool add(const GameClient* gc)
    {
        return _pool.add(gc);
    }

private:
    TcpConnectionPool _pool;
};

#define GCM GameClientMgr::getInstance()

} // namespace buf

#endif // GAMECLIENTMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

