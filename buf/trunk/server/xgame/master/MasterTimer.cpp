
#include "MasterTimer.h"
#include "MasterProcessor.h"
#include "MasterProcessorMgr.h"
#include "core/TcpProcess.h"
#include "core/Enumerator.h"
#include "core/Logger.h"

namespace buf
{

struct DPTEnumerator : public Enumerator<TcpProcess>
{
    virtual bool operator()(TcpProcess* ptr)
    {
        if (!ptr) return false;
        MasterProcessor* mp = reinterpret_cast<MasterProcessor*>(ptr);
        if (!mp->isRecycle())
            mp->doProcess();
        return true;
    }
};

static DPTEnumerator e;
bool DoProcessTimer::onTimer()
{
    static const timediff_t FRAME_MAX = 30;
    RTime rt;

#if 0
    // 对主动管理器连接进行消息处理
    MasterClient.doProcess();
    // 对主动数据库的连接进行消息处理
    RecordClient.doProcess();
    // 对主动会话连接进行消息处理  
    SessionClient.doProcess();
#endif

    // 对所有的被动连接进行消息处理
    MPMGR.enumerate(e);

    timediff_t elapse = rt.elapse();
    if (elapse > FRAME_MAX)
    {
        sLog.warn("帧超时: %lu(MAX:%lu)\n", elapse, FRAME_MAX);
    }

    return true;
}

void MasterTimer::init()
{
    addTimer(BUFNEW DoProcessTimer(25));
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

