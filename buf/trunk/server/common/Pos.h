
#ifndef POS_H_
#define POS_H_

#include "core/Config.h"
#include <vector>
#include <algorithm>

namespace buf
{

enum {
    POS_UP          = 0,
    POS_UPRIGHT     = 1,
    POS_RIGHT       = 2,
    POS_DOWNRIGHT   = 3,
    POS_DOWN        = 4,
    POS_DOWNLEFT    = 5,
    POS_LEFT        = 6,
    POS_UPLEFT      = 7,
    POS_UNKONW      = 8
};

// XXX: Pos.x * Pos.y
typedef u32_t PosI;

// XXX: 用于调整Pos
struct PosS
{
    int x;
    int y;
};

struct Pos
{
    u32_t x;
    u32_t y;

    Pos() : x(0), y(0) {}
    Pos(u32_t x, u32_t y) : x(x), y(y) {}
    Pos(const Pos& p) : x(p.x), y(p.y) {}

    Pos& operator=(const Pos& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    const Pos& operator+(const Pos& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    const Pos& operator-(const Pos& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    const Pos& operator+=(const Pos& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    const Pos& operator-=(const Pos& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    const Pos& operator+(const PosS& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    const Pos& operator-(const PosS& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    
    const Pos& operator+=(const PosS& p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    const Pos& operator-=(const PosS& p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    bool operator>(const Pos& p)
    {
        return (x > p.x && y > p.y);
    }

    bool operator>=(const Pos& p)
    {
        return (x >= p.x && y >= p.y);
    }

    inline bool operator<(const Pos& p)
    {
        return (x < p.x && y < p.y);
    }

    inline bool operator<=(const Pos& p)
    {
        return (x <= p.x && y <= p.y);
    }

    int dir(const Pos& p) const
    {
        if (x == p.x && y > p.y)
            return POS_UP;
        else if (x < p.x && y > p.y)
            return POS_UPRIGHT;
        else if (x < p.x && y == p.y)
            return POS_RIGHT;
        else if (x < p.x && y < p.y)
            return POS_DOWNRIGHT;
        else if (x == p.x && y < p.y)
            return POS_DOWN;
        else if (x > p.x && y < p.y)
            return POS_DOWNLEFT;
        else if (x > p.x && y == p.y)
            return POS_LEFT;
        else if (x > p.x && y > p.y)
            return POS_UPLEFT;
        return POS_UNKONW;
    }
};

typedef std::vector<Pos> PosVec_t;
typedef std::vector<PosS> PosSVec_t;
typedef std::vector<PosI> PosIVec_t;

inline void printvec(const PosIVec_t& v)
{
    PosIVec_t _v = v;
    std::sort(_v.begin(), _v.end());
    for (size_t i = 0; i < _v.size(); ++i)
    {
        printf("%d ", _v[i]);
    }
    printf("\n");
}

inline void printvec(const PosVec_t& v)
{
    for (size_t i = 0; i < v.size(); ++i)
    {
        printf("(%d,%d) ", v[i].x, v[i].y);
    }
    printf("\n");
}

inline static int getReverseDirect(int dir)
{
    (void)getReverseDirect;
    return (dir + 4) % 8;
}

static bool posInRange(const Pos& p1, const Pos& p2, u32_t radius)
{
    UNUSE(posInRange);
    if ((u32_t)abs(p1.x - p2.x) <= radius && (u32_t)abs(p1.y - p2.y) <= radius)
        return true;
    return false;
}

static bool posInRange(const Pos& p1, const Pos& p2, int wide, int height)
{
    //UNUSE(posInRange);
    if (abs(p1.x - p2.x) <= wide && abs(p1.y - p2.y) <= height)
        return true;
    return false;
}

static int getCmpDir(const Pos& mypos, const Pos& otherpos)
{
    (void)getCmpDir;
    int x = otherpos.x - mypos.x;
    int y = otherpos.y - mypos.y;

    int absx = abs(x);
    int absy = abs(y);

    if (absx > absy && absy < absx/2)
        y = 0;
    else if (absx < absy && absx < absy/2)
        x = 0;

    int dir = POS_UP;
    if (x < 0 &&  y < 0)
        dir = POS_UPLEFT;
    else if (x < 0 && 0 == y)
        dir = POS_LEFT;
    else if (x < 0 && y > 0)
        dir = POS_DOWNLEFT;
    else if (0 == x && y > 0)
        dir = POS_DOWN;
    else if (x > 0 && y > 0)
        dir = POS_DOWNRIGHT;
    else if (x > 0 && 0 == y)
        dir = POS_RIGHT;
    else if (x > 0 && y < 0)
        dir = POS_UPRIGHT;
    else if (0 == x && y < 0)
        dir = POS_UP;
    return dir;
}


} // namespace buf

#endif // POS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

