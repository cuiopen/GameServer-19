
#ifndef GAMEPROCESSORMGR_H_
#define GAMEPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class GameProcessorMgr : public TcpProcessMgr {};

#define MPMGR GameProcessorMgr::getInstance()

} // namespace buf

#endif // GAMEPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

