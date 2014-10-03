
#ifndef GATEWAYPROCESSORMGR_H_
#define GATEWAYPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class GatewayProcessorMgr : public TcpProcessMgr {};

#define MPMGR GatewayProcessorMgr::getInstance()

} // namespace buf

#endif // GATEWAYPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

