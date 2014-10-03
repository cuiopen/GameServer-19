
#ifndef COMMONCLIENT_H_
#define COMMONCLIENT_H_

#include "core/TcpClient.h"

namespace buf
{

class CommonClient : public TcpClient
{
public:
    CommonClient(const char* name, const char* ip, port_t port, s32_t type)
        : TcpClient(name, ip, port), _type(type) {}

    virtual ~CommonClient() {}
    virtual void onFinal();

    inline s32_t getType() const { return _type; }
    inline bool connect2(const char* sn = NULL)
    {
        if (!connect(sn, true))
            return false;
        return true;
    }

private:
    s32_t _type;
};

} // namespace buf

#endif // COMMONCLIENT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

