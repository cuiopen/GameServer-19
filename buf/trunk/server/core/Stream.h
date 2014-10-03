
#ifndef STREAM_H_
#define STREAM_H_

#include "Config.h"
#include "TypeTraits.h"
#include "BufferBase.h"

#include <vector>
#include <string>

namespace buf
{

template <typename T>
class Stream : public BufferBase
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::vector<value_type> container;
    typedef typename container::size_type size_type;

public:
#if defined(DEFAULT_STREAM_SIZE) &&  DEFAULT_STREAM_SIZE
    static const size_type DEFAULT_SIZE = DEFAULT_STREAM_SIZE;
#else
    static const size_type DEFAULT_SIZE = 4096;
#endif

public:
    Stream() : BufferBase(DEFAULT_SIZE), _data(DEFAULT_SIZE)
    {}

    Stream(size_type size) : BufferBase(size), _data(size)
    {}

    Stream(const Stream& s) : BufferBase(s._max, s.curr(), s.off()), _data(s._data)
    {
        rflip();
    }

    Stream(const_pointer data, size_type count)
        : BufferBase(DEFAULT_SIZE>count?DEFAULT_SIZE:count, count)
    {
        append(data, count);
    }

    ~Stream()
    {
        clear();
    }

    inline Stream& put(const_pointer v, size_type c)
    {
        return append(v, c);
    }

    inline Stream& append(const_pointer v, size_type c)
    {
        checksize(c);
        MEMCPY(&_data[_curr], v, c*sizeof(value_type));
        _curr += c;
        return *this;
    }

    inline Stream& read(pointer v, size_type c)
    {
        if (rsize() >= c)
        {
            MEMCPY(v, &_data[_off], c*sizeof(value_type));
            _off += c;
        }
        else
        {
            MEMCPY(v, &_data[_off], rsize()*sizeof(value_type));
            _off = _curr;
        }
        return *this;
    }

    inline Stream& read(value_type& v)
    {
        if (!rsize())
        {
            static value_type t;
            v = t;
        }
        else
        {
            v = _data[_off];
            ++_off;
        }
        return *this;
    }

    inline Stream& operator<<(const value_type& v)
    {
        return append(reinterpret_cast<const_pointer>(&v), 1);
    }

    inline Stream& operator>>(value_type& v)
    {
        return read(v);
    }

    // XXX: do not call remove() frequently
    inline void remove(size_t s, size_t n)
    {
        if (_off+s >= _max)
            return;
        if (_off+s+n > _max)
            n = _max-s-_off;

        if (_off+s+n >= _curr)
        {
            _curr = _off+s;
        }
        else
        {
            MEMMOVE(&_data[0]+_off+s, &_data[0]+_off+s+n, n);
            _curr -= n;
        }
    }

    inline value_type& at(size_type i)
    {
        if (i+_off > _curr)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    inline const value_type& at(size_type i) const
    {
        if (i+_off > _curr)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
    inline value_type& operator[](size_type i)
    {
        if (i+_off > _curr)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    inline const value_type& operator[](size_type i) const
    {
        if (i+_off > _curr)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    inline const_pointer operator&() const { return &_data[_off]; }

    // XXX: Copied from Buffer.h
    // XXX: You do not save the value returned by rbuf and wbuf
    // XXX: rbuf and wbuf will be used as function but not inline
    inline const_pointer rbuf() const { return &_data[_off]; }
    inline pointer wbuf() { return &_data[_curr]; }

    // XXX: MEMCPY
    inline void rflip(size_type size = 0)
    {
        _off += size;
        if (_off < _curr)
        {
            if (_off)
            {
                size_type nsize = _curr - _off;
                MEMCPY(&_data[0], &_data[_off], nsize*sizeof(value_type));
                _off = 0;
                _curr = nsize;
            }
        } else {
            _off = _curr = 0;
        }
    }

    inline void clear()
    {
        BufferBase::clear();
        _data.clear();
    }

    inline void swap(container c)
    {
        _data.swap(c);
        _off = 0;
        _curr = 0;
        _max = c.size();
    }

    inline void swap(Stream& s)
    {
        SWAP(_off, s._off);
        SWAP(_curr, s._curr);
        SWAP(_max, s._max);

        s._data.swap(_data);
        s.rflip();
        rflip();
    }

private:
    inline void checksize(size_type size)
    {
        if (wsize() < size + 8)
        {
            unsigned int nsize = max() + DEFAULT_SIZE * ((DEFAULT_SIZE+size+8)/DEFAULT_SIZE);
            _data.reserve(nsize); // XXX: &_data[N]
            _max = nsize;
        }
        return;
    }

private:
    container _data;
};

} // namespace buf

#endif // STREAM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

