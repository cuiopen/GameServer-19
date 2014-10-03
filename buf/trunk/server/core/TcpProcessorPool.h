
#ifndef TCPPROCESSORPOOL_H_
#define TCPPROCESSORPOOL_H_

#include "TcpProcessPool.h"

namespace buf
{

static const struct TPConfig TPP_MAX[] = {
    {3, 512, "ACK"},
    {1, 512, "SYNC"},
    {8, 512, "PROCSSING"},
    {1, 512, "RECYCLE"},
    {0,0,""},
};

class TcpProcessorPool : public TcpProcessPool
{
public:
    TcpProcessorPool() {}
    virtual ~TcpProcessorPool() {}

    virtual bool init(const TPConfig* cfg = TPP_MAX);
    virtual bool addAckEvent(EventManager& em, const TcpProcess* tp, const StateThread* st);
    virtual bool addProcessingEvent(EventManager& em, const TcpProcess* tp, const StateThread* st);
};

} // namespace buf

#endif // TCPPROCESSORPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

