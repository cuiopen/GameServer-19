
#ifndef COUNTER_H_
#define COUNTER_H_

#include "Atomic.h"
#include "Logger.h"

namespace buf
{

class Counter
{
public:
    Counter(const char* msg) : _count(0)
    {
        if (msg)
        {
            fprintf(stderr, "%s", msg);
            sLog.info("%s ", msg);
        }
    }

    ~Counter()
    {}

    inline int operator++()
    {
        ++_count;
        return _count;
    }

    inline int operator++(int)
    {
        return _count++;
    }

    void finish()
    {
        fprintf(stderr, "%d counts\n", (int)_count);
        sLog.info("%d counts", (int)_count);
        _count = 0;
    }

private:
    Atomic _count;
};

} // namespace buf

#endif // COUNTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

