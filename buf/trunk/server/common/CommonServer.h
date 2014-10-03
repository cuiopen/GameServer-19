
#ifndef COMMONSERVER_H_
#define COMMONSERVER_H_

#include "core/TcpServer.h"
#include "script/Script.h"
#include "Servers.h"

namespace buf
{

class CommonServer : public TcpServer
{
public:
    static bool getServerCfgBaseInfo(Table& cfg, ServerCfgBaseInfo& info);
    static bool getIpMask(Table& mask);
    static bool getLogCfg(Table& log, CommonServer* cs);

public:
    CommonServer(const char* name, s16_t type, s16_t id)
        : TcpServer(name), _type(type), _id(id) {}
    virtual ~CommonServer() {}

    // scene and gateway will overide this function
    virtual bool init(const ServerCfgBaseInfo& info);

    inline void setType(s16_t type) { _type = type; }
    inline s16_t getType() const { return _type; }

    inline void setId(s16_t id) { _id = id; }
    inline s16_t getId() const { return _id; }

private:
    s16_t _type;
    s16_t _id;
};

} // namespace buf

#endif // COMMONSERVER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

