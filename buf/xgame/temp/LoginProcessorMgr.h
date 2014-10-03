
#ifndef LOGINPROCESSORMGR_H_
#define LOGINPROCESSORMGR_H_

#include "core/TcpProcessMgr.h"

namespace buf
{

class LoginProcessorMgr : public TcpProcessMgr {};

#define MPMGR LoginProcessorMgr::getInstance()

} // namespace buf

#endif // LOGINPROCESSORMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

