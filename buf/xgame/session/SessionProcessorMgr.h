
#ifndef SESSIONPROCESSORMGR_H_
#define SESSIONPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class SessionProcessorMgr : public TcpProcessMgr {};

#define MPMGR SessionProcessorMgr::getInstance()

} // namespace buf

#endif // SESSIONPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

