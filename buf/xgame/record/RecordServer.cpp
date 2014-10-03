
#include "RecordServer.h"
#include "RecordProcessor.h"
#include "common/Cfg.h"

namespace buf
{

RecordServer::RecordServer(const char* name) : SlaveServer(name, SERVER_RECORD, 0)
{}

RecordServer::~RecordServer()
{}

bool RecordServer::init()
{
    if (!SlaveServer::init())
        return false;
    return true;
}

TcpProcessor* RecordServer::createProcessor(socket_t s, sockaddrin_t* sa) 
{
    if (s < 0 || !sa)
        return 0;
    return new RecordProcessor(this, s, sa);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

