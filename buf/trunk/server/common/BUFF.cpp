
#include "BUFF.h"

namespace buf
{

BUFFMgr::~BUFFMgr()
{
    for (iterator i = _buffs.begin(), e = _buffs.end(); i != e; ++i)
        DELETE(i->second);
}

bool BUFFMgr::addBuff(u32_t status, u32_t endtime, bool notify)
{
    if (_buffs.size())
    {
        iterator i = _buffs.find(status);
        if (i != _buffs.end())
            clearBuff(status, notify);
    }

    BUFF* buff = newBuff(status, endtime);
    if (!buff)
        return false;

    if (notify)
        onNotify(buff, ADD);
    _buffs[status] = buff;
    return true;
}

bool BUFFMgr::clearBuff(u32_t status, bool notify)
{
    iterator i = _buffs.find(status);
    if (i == _buffs.end())
        return false;
    if (notify)
        onNotify(i->second, CLEAR);
    DELETE(i->second);
    _buffs.erase(i);
    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

