
#ifndef SERVERS_H_
#define SERVERS_H_

#include "core/Socket.h"
#include "common/Packet.h"

namespace buf
{

#define SERVER_UNKNOWN       0  // XXX: client
#define SERVER_NONE         'N' // XXX: Slave
#define SERVER_MASTER       11
#define SERVER_GAME         22
#define SERVER_RECORD       33
#ifdef USE_SERVER_LOGIN
#define SERVER_LOGIN        44
#endif
#ifdef USE_SERVER_GATEWAY
#define SERVER_GATEWAY      55
#endif
#ifdef USE_SERVER_SESSION
#define SERVER_SESSION      66
#endif

struct ServerCfgBaseInfo
{
    ServerCfgBaseInfo(s32_t type = 0, s32_t id = 1)
        : type(type), id(id), port(0), extport(0),
        ack(0), ack_tasks(0), sync(0),
        processing(0), processing_tasks(0), recycle(0) {}

    s32_t type;
    s32_t id;
    port_t port;
    port_t extport;
    std::string ip;
    std::string extip;

    s32_t ack;
    s32_t ack_tasks;
    s32_t sync;
    s32_t processing;
    s32_t processing_tasks;
    s32_t recycle;

    std::string config;

    void operator<<(Packet& bb)
    {
        bb >> type;
        bb >> id;
        bb >> port;
        bb >> extport;
        bb >> ip;
        bb >> extip;
        bb >> ack;
        bb >> ack_tasks;
        bb >> sync;
        bb >> processing;
        bb >> processing_tasks;
        bb >> recycle;
        bb >> config;
    }

    void operator>>(Packet& bb)
    {
        bb << type;
        bb << id;
        bb << port;
        bb << extport;
        bb << ip;
        bb << extip;
        bb << ack;
        bb << ack_tasks;
        bb << sync;
        bb << processing;
        bb << processing_tasks;
        bb << recycle;
        bb << config;
    }
};

extern const char* getServerTypeString(s32_t type);

} // namespace buf

#endif // SERVERS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

