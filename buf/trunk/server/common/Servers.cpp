
#include "Servers.h"

namespace buf
{

const char* getServerTypeString(s32_t type)
{
    switch (type) {
        case SERVER_MASTER:
            return "master";
        case SERVER_GAME:
            return "game";
        case SERVER_RECORD:
            return "record";
        case SERVER_SESSION:
            return "session";
        case SERVER_LOGIN:
            return "login";
        case SERVER_GATEWAY:
            return "gateway";
        default:
            break;
    }
    return "null";
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

