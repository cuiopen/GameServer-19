
#ifndef RECORDPROCESSORMGR_H_
#define RECORDPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class RecordProcessorMgr : public TcpProcessMgr {};

#define MPMGR RecordProcessorMgr::getInstance()

} // namespace buf

#endif // RECORDPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

