
#ifndef TCPCONNECTIONPOOL_H_
#define TCPCONNECTIONPOOL_H_

#include "TcpProcessPool.h"

namespace buf
{

class TcpConnectionPool : public TcpProcessPool
{
public:
    TcpConnectionPool() {}
    virtual ~TcpConnectionPool() {}

    virtual bool init(const TPConfig* config = NULL);
    virtual bool addAckEvent(EventManager& em, const TcpProcess* tp, const StateThread* st);
    virtual bool addProcessingEvent(EventManager& em, const TcpProcess* tp, const StateThread* st);
};

} // namespace buf

#endif // TCPCONNECTIONPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

