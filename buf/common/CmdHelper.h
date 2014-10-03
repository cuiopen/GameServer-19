
#ifndef CMDHELPER_H_
#define CMDHELPER_H_

#include "core/TypeTraits.h" // XXX: for nasty_cast
#include "core/TcpProcess.h"
#include "core/RTime.h"
#include "core/Logger.h"

typedef u16_t CMD_t;
#define CMD(type,cmd) ((CMD_t)((type)<<8|(cmd)))
#define CMDTYPE(c) (((c)>>8)&0xFF)
#define CMDCMD(c) ((c)&0xFF)
#ifndef CMDMAX
#define CMDMAX CMD(0xFF,0xFF)
#endif
#define CMDREG(cmd, call) CmdHandlerRegister<cmd>(call)

#define CAT(x,y) x##y
#define STR(x) #x

/**
 * call must be 
 *      void (*)(TcpProcess*, const Body*, size_t)
 *          Or
 *      void (*)(TcpProcess*, const Body&)
 **/
#define CMDDEF(name, type, cmd, call)                                   \
    static const CMD_t name = CMD(type, cmd);                           \
    class name##_##type##_##cmd                                         \
    {                                                                   \
        public:                                                         \
            name##_##type##_##cmd()                                     \
            {                                                           \
                CMDREG(name, call);                                     \
                KCMD_HANDLERS[name].cmdname = ""#name"("#type","#cmd")";\
            } \
    } static __##name##_##type##_##cmd

namespace buf
{

struct CmdProxy
{
    const char* cmdname;
    int (*proxy)(TcpProcess* tp, const void* cmd, size_t len, void* call);
    void* call; // must be not a member of class
};

static CmdProxy KCMD_HANDLERS[CMDMAX];

template <typename Body>
inline static int CmdHandlerProxy(TcpProcess* tp, const void* cmd, size_t len, void* call)
{
    if (len < sizeof(Body))
    {
        // TODO: warn log
        return -1;
    }
    return (nasty_cast<int (*)(TcpProcess*, const Body&)>(call))(tp, *(Body*)(cmd));
}

template <CMD_t cmdid, typename Body>
inline static int CmdHandlerProxy(TcpProcess* tp, const void* cmd, size_t len, void* call)
{
    return (nasty_cast<int (*)(TcpProcess*, const Body*, size_t)>(call))(tp, (const Body*)(cmd), len);
}

template <CMD_t cmdid, typename Body>
static bool CmdHandlerRegister(int (*call)(TcpProcess* tp, const Body& body))
{
    ASSERT(!KCMD_HANDLERS[cmdid].proxy);
    ASSERT(cmdid < CMDMAX);
    KCMD_HANDLERS[cmdid].proxy = &CmdHandlerProxy<Body>;
    KCMD_HANDLERS[cmdid].call = nasty_cast<void*>(call);
    return true;
}

template <CMD_t cmdid, typename Body>
static bool CmdHandlerRegister(int (*call)(TcpProcess* tp, const Body* body, size_t len))
{
    ASSERT(!KCMD_HANDLERS[cmdid].proxy);
    ASSERT(cmdid < CMDMAX);
    KCMD_HANDLERS[cmdid].proxy = &CmdHandlerProxy<cmdid, Body>;
    KCMD_HANDLERS[cmdid].call = nasty_cast<void*>(call);
    return true;
}

static const char* GetCmdName(CMD_t cmdid)
{
    if (cmdid >= CMDMAX)
        return 0;
    return KCMD_HANDLERS[cmdid].cmdname;
}

static int CmdHandlerDoProcess(TcpProcess* tp, const void* cmd, size_t len)
{
    (void)CmdHandlerDoProcess;
#ifndef _NDEBUG
    if (!cmd || !len)
#else
    if (!tp || !cmd || !len)
#endif
        return -1;
    if (len < sizeof(CMD_t))
    {
        // TODO: warn log
        return -1;
    }
    CMD_t cmdid = *(CMD_t*)(cmd);
    if (cmdid >= CMDMAX)
    {
        // TODO: warn log
        return -1;
    }
    if (!KCMD_HANDLERS[cmdid].proxy || !KCMD_HANDLERS[cmdid].call)
        return -1;

    // XXX:
    {
        TimeFilm tf(GetCmdName(cmdid), &sLog);
        return KCMD_HANDLERS[cmdid].proxy(tp, reinterpret_cast<const char*>(cmd)+sizeof(CMD_t), len-sizeof(CMD_t), KCMD_HANDLERS[cmdid].call);
    }

    return -1;
}

} // namespace buf

#endif // CMDHELPER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

