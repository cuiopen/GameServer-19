
#include "TcpProcessorPool.h"
#include "TcpProcessor.h"

namespace buf
{

bool TcpProcessorPool::init(const TPConfig* cfg)
{
    return TcpProcessPool::init(cfg);
}

bool TcpProcessorPool::addAckEvent(EventManager& em, const TcpProcess* tp, const StateThread* st)
{
    return em.addEvent(BUFNEW TcpProcessorAckEvent(tp->fd(), EVENT_ALL, tp, st));
}

bool TcpProcessorPool::addProcessingEvent(EventManager& em, const TcpProcess* tp, const StateThread* st)
{
    return em.addEvent(BUFNEW TcpProcessorProcessingEvent(tp->fd(), EVENT_ALL, tp, st));
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

