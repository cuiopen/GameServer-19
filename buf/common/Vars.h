
#ifndef VARS_H_
#define VARS_H_

#include "core/Config.h"
#include "core/RTime.h"
#include "core/Buffer.h"
#include "core/UnorderedContainer.h"

namespace buf
{

typedef enum
{
    VARRECYCLE_NONE = -1,

    // Calendar
    VARRECYCLE_YEAR, // Calendar year
    VARRECYCLE_MONTH,
    VARRECYCLE_WEEK,
    VARRECYCLE_DAY,
    VARRECYCLE_HOUR,
    VARRECYCLE_MIN,

    // Absolute
    VARRECYCLE_WEEK_ABS,
    VARRECYCLE_DAY_ABS,
    VARRECYCLE_HOUR_ABS,
    VARRECYCLE_MIN_ABS,

    VARRECYCLE_MAX
} VarRecycle_e;

// XXX: K and V must be in u8_t,u16_t,u32_t,u64_t

template <typename K = u16_t, typename V = u32_t>
class Vars
{
public:
    struct Var
    {
        V val;
        time_t end;
    };

    struct SerVar
    {
        K var;
        V val;
        time_t end;
    };

    typedef typename UNORDERED_MAP<K, Var> container;
    typedef typename container::value_type value_type;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

public:
    Vars() {}
    ~Vars() { _vars.clear(); }

    V getVar(K var)
    {
        iterator i = _vars.find(var);
        if (i != _vars.end())
        {
            time_t now = Time::Now();
            if (i->second.end && now >= i->second.end)
            {
                i->second.val = 0;
                return 0;
            }
            return i->second.val;
        }
        return 0;
    }

    void addVar(K var, V val, VarRecycle_e recycle = VARRECYCLE_MAX)
    {
        iterator i = _vars.find(var);
        if (i != _vars.end())
        {
            time_t now = Time::Now();
            if (i->second.end && now >= i->second.end)
                i->second.val = 0;

            i->second.val += val;
        }
        else
            setVar(var, val, recycle);
    }

    void setVar(K var, V val, VarRecycle_e recycle = VARRECYCLE_MAX)
    {
        time_t now = Time::Now();
        iterator i = _vars.find(var);
        if (i != _vars.end())
        {
            i->second.val = val;

            switch (recycle)
            {
                case VARRECYCLE_NONE:
                    i->second.end = 0;
                    break;
                case VARRECYCLE_YEAR:
                    i->second.end = Time::theYear(1,now);
                    break;
                case VARRECYCLE_MONTH:
                    i->second.end = Time::theMonth(1,now);
                    break;
                case VARRECYCLE_WEEK:
                    i->second.end = Time::theWeek(1,now);
                    break;
                case VARRECYCLE_DAY:
                    i->second.end = Time::theDay(1,now);
                    break;
                case VARRECYCLE_HOUR:
                    i->second.end = Time::theHour(1,now);
                    break;
                case VARRECYCLE_MIN:
                    i->second.end = Time::theMin(1,now);
                    break;

                case VARRECYCLE_WEEK_ABS:
                    i->second.end = now + TIME_WEEK;
                    break;
                case VARRECYCLE_DAY_ABS:
                    i->second.end = now + TIME_DAY;
                    break;
                case VARRECYCLE_HOUR_ABS:
                    i->second.end = now + TIME_HOUR;
                    break;
                case VARRECYCLE_MIN_ABS:
                    i->second.end = now + TIME_MIN;
                    break;
                default:
                    i->second.end = 0;
                    break;
            }
        } else { // if (i != _vars.end())
            Var v;
            v.val = val;

            if (recycle >= VARRECYCLE_MAX)
                recycle = VARRECYCLE_NONE;
            v.end = 0;

            switch (recycle)
            {
                case VARRECYCLE_NONE:
                    v.end = 0;
                    break;
                case VARRECYCLE_YEAR:
                    v.end = Time::theYear(1,now);
                    break;
                case VARRECYCLE_MONTH:
                    v.end = Time::theMonth(1,now);
                    break;
                case VARRECYCLE_WEEK:
                    v.end = Time::theWeek(1,now);
                    break;
                case VARRECYCLE_DAY:
                    v.end = Time::theDay(1,now);
                    break;
                case VARRECYCLE_HOUR:
                    v.end = Time::theHour(1,now);
                    break;
                case VARRECYCLE_MIN:
                    v.end = Time::theMin(1,now);
                    break;

                case VARRECYCLE_WEEK_ABS:
                    v.end = now + TIME_WEEK;
                    break;
                case VARRECYCLE_DAY_ABS:
                    v.end = now + TIME_DAY;
                    break;
                case VARRECYCLE_HOUR_ABS:
                    v.end = now + TIME_HOUR;
                    break;
                case VARRECYCLE_MIN_ABS:
                    v.end = now + TIME_MIN;
                    break;

                default:
                    v.end = 0;
                    break;
            }

            _vars[var] = v;
        }
    }

    bool delVar(K var)
    {
        iterator i = _vars.find(var);
        if (i != _vars.end())
        {
            _vars.erase(var);
            return true;
        }
        return false;
    }

    size_t serialize(Buffer& buf)
    {
        SerVar v = {0,0,0};
        size_t s = 0;
        for (iterator i = _vars.begin(), e = _vars.end(); i != e; ++i)
        {
            time_t now = Time::Now();
            if (!i->second.end || now < i->second.end)
            {
                v.var = i->first;
                v.val = i->second.val;
                v.end = i->second.end;
                buf.put(reinterpret_cast<Buffer::const_pointer>(&v), sizeof(v));
                s += sizeof(v);
            }
        }
        return s;
    }

    void unserialize(const Buffer& buf)
    {
        if (!buf.rsize())
            return;
        if (buf.rsize() < sizeof(SerVar))
            return;

        Var v;
        size_t n = buf.rsize() / sizeof(SerVar);
        Buffer::pointer p = const_cast<Buffer::pointer>(buf.rbuf());
        SerVar* pv = reinterpret_cast<SerVar*>(p);
        while (n)
        {
            v.val = pv->val;
            v.end = pv->end;
            time_t now = Time::Now();
            if (!v.end || now < v.end)
                _vars[pv->var] = v;
            --n;
        }
        return;
    }

    void clear()
    {
        _vars.clear();
    }

protected:
    container _vars;
};

// VAR|VALUE|ENDTIME

} // namespace buf

#endif // VARS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

