
#include "SessionTimer.h"
#include "SessionProcessor.h"
#include "SessionProcessorMgr.h"
#include "core/TcpProcess.h"
#include "core/Enumerator.h"

namespace buf
{

struct DPTEnumerator : public Enumerator<TcpProcess>
{
    virtual bool operator()(TcpProcess* ptr)
    {
        if (!ptr) return false;
        SessionProcessor* mp = reinterpret_cast<SessionProcessor*>(ptr);
        if (!mp->isRecycle())
            mp->doProcess();
        return true;
    }
};

static DPTEnumerator e;
bool DoProcessTimer::onTimer()
{
#if 0
    // 对主动数据库的连接进行消息处理
    RecordClient.doProcess();
#endif

    // 对所有的被动连接进行消息处理
    MPMGR.enumerate(e);

    return true;
}

void SessionTimer::init()
{
    addTimer(BUFNEW DoProcessTimer(25));
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

