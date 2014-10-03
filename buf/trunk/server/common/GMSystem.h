
#ifndef GMSYSTEM_H_
#define GMSYSTEM_H_

#include "core/Config.h"
#include "core/Tokenizer.h"

// XXX: It's just a template

namespace buf
{

#define GMT_PLAYER  0x01
#define GMT_NORMAL  0x02
#define GMT_ADMIN   0x04
#define CMT_SUPER   0x08
#define GMT_DEBUG   0x10
#define GMT_ALL     0x1F

struct GMHandler
{
    const char* name;
    void (*handler)(const Tokenizer& params);
    u8_t mode;
    const char* desc;
};

void GMParse(GMHandler handlers[], int nhandler, const char* cmd, int mode = GMT_ALL);

} // namespace buf

#endif // GMSYSTEM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

