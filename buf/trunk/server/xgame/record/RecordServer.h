
#ifndef RECORDSERVER_H_
#define RECORDSERVER_H_

#include "common/SlaveServer.h"

namespace buf
{

class RecordServer : public SlaveServer
{
public:
    RecordServer(const char* name);
    virtual ~RecordServer();

    virtual bool init();

    inline bool ack(s32_t type, const char* ip, ServerCfgBaseInfo*& info)
    {
        return true;
    }

    inline bool sync(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool process(s32_t type, s32_t id)
    {
        return true;
    }

    inline bool recycle(s32_t type, s32_t id)
    {
        return true;
    }

    virtual TcpProcessor* createProcessor(socket_t s, sockaddrin_t* sa);

private:
};

} // namespace buf

#endif // RECORDSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

