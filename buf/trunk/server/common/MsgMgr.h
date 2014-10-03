
#ifndef MSGMGR_H_
#define MSGMGR_H_

#include "core/Config.h"
#include "core/UnorderedContainer.h"

#include <map>
#include <string>

namespace buf
{

struct MsgItem
{
    u8_t channel;
    u32_t color;
    std::string msg;

    MsgItem& operator=(const MsgItem& mi)
    {
        channel = mi.channel;
        color = mi.color;
        msg = mi.msg;
        return *this;
    }
};

class MsgMgr
{
public:
    typedef std::map<u32_t, MsgItem> MsgMap_t;
    typedef MsgMap_t::iterator iterator;
    typedef MsgMap_t::const_iterator const_iterator;

public:
    MsgMgr() {}
    ~MsgMgr() { _msgs.clear(); }

    const MsgItem* get(u32_t msgid)
    {
        iterator i = _msgs.find(msgid);
        if (i == _msgs.end())
            return 0;
        return &i->second;
    }

    void set(u32_t msgid, u8_t channel, u32_t color, const char* msg)
    {
        if (!msgid || !msg)
            return;

        MsgItem mi;
        mi.channel = channel;
        mi.color = color;
        mi.msg  = msg;
        set(msgid, &mi);
    }

    void set(u32_t msgid, const MsgItem* msg)
    {
        if (!msgid || !msg)
            return;
        iterator i = _msgs.find(msgid);
        if (i != _msgs.end())
            _msgs.erase(msgid);
        _msgs[msgid] = *msg;
    }

    // XXX: only message
    inline void set(u32_t msgid, const char* msg)
    {
        return set(msgid, 0, 0, msg);
    }

private:
    MsgMap_t _msgs;
};

} // namespace buf

#endif // MSGMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

