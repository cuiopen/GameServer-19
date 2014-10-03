
#ifndef MASTERPROCESSORMGR_H_
#define MASTERPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class MasterProcessorMgr : public TcpProcessMgr {};

#define MPMGR MasterProcessorMgr::getInstance()

} // namespace buf

#endif // MASTERPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

