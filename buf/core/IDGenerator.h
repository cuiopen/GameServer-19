
#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_

#include "Mutex.h"
#include "Random.h"
#include "Atomic.h"
#include "TypeTraits.h"

#include <string>
#include <set>

namespace buf
{

// XXX: IDGenerator only in memory
class IDGenerator
{
public:
    typedef Atomic::atomic_type key_type;
    typedef std::set<key_type> container;
#ifdef __APPLE__
    typedef container::iterator iterator;
#else
    typedef typename container::iterator iterator;
#endif

public:
    IDGenerator(const key_type& min = 1, const key_type& max = -1)
        : _min(min), _max(max), _curMax(_min), _lock()
    {
        max_init();
    }

    inline void max_init(void)
    {
        static u64_t mask[] = {0x7f, 0x7fff, 0x7fffffff, 0x7fffffffffffffff};

        int m = 0;
        size_t s = sizeof(key_type);
        while (s>>=1) m++;
        if (!is_unsigned<key_type>::value)
            _max &= mask[m];
    }

    virtual ~IDGenerator()
    {
        ScopeLock lock(_lock);
        _vals.clear();
    }

    virtual key_type get()
    {
        key_type ret = _max;

        if ((key_type)_curMax >= _max)
        {
            size_t size = _vals.size();
            if (!_vals.empty() && size)
            {
                int idx = Random()(0, size-1);
#ifdef _DEBUG
                printf("size: %u, idx: %d\n", size, idx);
#endif
                ScopeLock lock(_lock);
#if defined(CXX0X) && CXX0X
                auto i = _vals.begin();
#else
#ifdef __APPLE__
                std::set<key_type>::iterator i = _vals.begin();
#else
                typename std::set<key_type>::iterator i = _vals.begin();
#endif
#endif
                std::advance(i, idx);
                if (i != _vals.end())
                {
                    ret = *i;
                    _vals.erase(i);
                }
            }
        }
        else
        {
            ret = _curMax;
            _curMax++;
        }
        return ret;
    }

    virtual bool put(const key_type& val)
    {
        bool ret = false;

        ScopeLock lock(_lock);
#ifdef __APPLE__
        std::set<key_type>::iterator i = _vals.find(val);
#else
        typename std::set<key_type>::iterator i = _vals.find(val);
#endif
        if (i == _vals.end())
        {
            ret = _vals.insert(val).second;
        }
        return ret;
    }

    inline key_type max() const { return _max; }
    inline key_type invalid() const { return _max; }

protected:
    key_type _min;
    key_type _max;
    Atomic _curMax;
    Mutex _lock;
    container _vals;
};

class RandIDGenerator : public IDGenerator
{
public:
#ifdef __APPLE__
    typedef IDGenerator::key_type key_type;
#else
    typedef typename IDGenerator::key_type key_type;
#endif
    using IDGenerator::_max;
    using IDGenerator::_min;

public:
    RandIDGenerator() : IDGenerator() {}
    ~RandIDGenerator() {}

public:
    virtual key_type get()
    {
        key_type ret = _max;
        ret = Random()(_min, _max);
        return ret;
    }

    virtual bool put(const key_type& val)
    {
        return false;
    }
};

} // namespace buf


#endif // IDGENERATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

