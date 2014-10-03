
#ifndef BUFF_H_
#define BUFF_H_

#include "core/Config.h"
#include <time.h>
#include <map>

namespace buf
{

class BUFF
{
public:
    BUFF() : _status(0), _endTime(0) {}
    BUFF(u32_t status, u32_t end) : _status(status), _endTime(end) {}

    inline u32_t getStatus() const { return _status; }
    inline u32_t getEndTime() const { return _endTime; }
    inline void setStatus(u32_t status) { _status = status; }
    inline void setEndTime(u32_t time) { _endTime = time; }
    inline bool outOfTime() const { return _endTime >= time(0); }

    virtual void effect() {}

private:
    u32_t _status;
    u32_t _endTime;
};

class BUFFMgr
{
public:
    static const int ADD = 1;
    static const int CLEAR = 2;

public:
    typedef std::map<u32_t, BUFF*> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    BUFFMgr() {}
    virtual ~BUFFMgr();

    bool addBuff(u32_t status, u32_t endtime, bool notify = true);
    bool clearBuff(u32_t status, bool notify = true);

    virtual void onNotify(BUFF* buff, int type) {UNUSE(buff);UNUSE(type);}
    virtual BUFF* newBuff(u32_t status, u32_t endtime) { UNUSE(status);UNUSE(endtime);return 0; }

protected:
    container _buffs;
};

} // namespace buf

#endif // BUFF_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

