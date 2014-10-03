
#include "TcpConnectionPool.h"
#include "TcpConnection.h"

namespace buf
{

static const struct TPConfig TPP_MAX[] = {
    {8, 512, "ACK"},
    {0, 0, "SYNC"},
    {8, 512, "PROCSSING"},
    {1, 512, "RECYCLE"},
    {0,0,""}
};

bool TcpConnectionPool::init(const TPConfig* config)
{
    if (config)
        return TcpProcessPool::init(config);
    else
        return TcpProcessPool::init(TPP_MAX);
}

bool TcpConnectionPool::addAckEvent(EventManager& em, const TcpProcess* tp, const StateThread* st)
{
    return em.addEvent(BUFNEW TcpConnectionAckEvent(tp->fd(), EVENT_ALL, tp, st));
}

bool TcpConnectionPool::addProcessingEvent(EventManager& em, const TcpProcess* tp, const StateThread* st)
{
    return em.addEvent(BUFNEW TcpConnectionProcessingEvent(tp->fd(), EVENT_ALL, tp, st));
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

