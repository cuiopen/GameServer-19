
#ifndef SLAVESERVER_H_
#define SLAVESERVER_H_

#include "CommonServer.h"
#include "core/TcpClient.h"

namespace buf
{

class SlaveServer : public CommonServer
{
public:
    SlaveServer(const char* name, int type, int id);
    virtual ~SlaveServer();

    virtual bool init();
    virtual void final();

private:
    bool recvCfg(ServerCfgBaseInfo& cfg);

private:
    TcpClient* _master;
};

} // namespace buf

#endif // SLAVESERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

