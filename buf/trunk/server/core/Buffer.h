
#ifndef BUFFER_H_
#define BUFFER_H_

#include "Config.h"
#include "BufferBase.h"
#include <vector>
#include <string>

namespace buf
{

class BufferMem : public BufferBase
{
public:
    typedef u8_t value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

public:
    BufferMem(size_type size, size_type curr = 0, size_type off = 0)
        : BufferBase(size, curr, off) {}

protected:
    pointer _data;
};

template <size_t SIZE = TRUNK_SIZE>
class BufferStack : public BufferBase
{
public:
    typedef u8_t value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

public:
    BufferStack(size_type size, size_type curr = 0, size_type off = 0)
        : BufferBase(size, curr, off) {}

    inline bool checksize(size_type size) { return (wsize() >= size); }

protected:
    value_type _data[SIZE];
};

class BufferContainer : public BufferBase
{
public:
    typedef std::vector<u8_t> container;
    typedef container::pointer pointer;
    typedef container::const_pointer const_pointer;
    typedef container::value_type value_type;

public:
    BufferContainer(size_type size, size_type curr = 0, size_type off = 0)
        : BufferBase(size, curr, off), _data(size) {}

    inline bool checksize(size_type size)
    {
        resize(size);
        return true;
    }

    inline void resize(size_type size)
    {
        if (wsize() < size + 8)
        {
            unsigned int nsize = max() + TRUNK_SIZE * ((TRUNK_SIZE+size+8)/TRUNK_SIZE);
            _data.resize(nsize, 0); // XXX: &_data[N]
            _max = nsize;
        }
    }

protected:
    container _data;
};

template <typename T = BufferContainer>
class BufferReader : public T
{
public:
    typedef typename T::size_type size_type;
    typedef typename T::value_type value_type;
    typedef typename T::const_pointer const_pointer;
    typedef typename T::pointer pointer;

public:
    using T::_data;
    using T::_off;
    using T::_curr;
    using T::_max;

    using T::rsize;
    using T::wsize;
    using T::empty;
    using T::clear;

public:
    BufferReader(size_type size, size_type curr = 0, size_type off = 0) : T(size, curr, off) {}

    // XXX: you do not save the value returned by rbuf and wbuf
    // XXX: rbuf and wbuf will be used as function but not inline
    inline const_pointer rbuf() const { return &_data[_off]; }
    inline pointer rbuf() { return &_data[_off]; }

    inline const_pointer wbuf() const { return &_data[_curr]; }
    inline pointer wbuf() { return &_data[_curr]; }

    inline char* c_rbuf() { return reinterpret_cast<char*>(&_data[_off]); }
    inline char* c_wbuf() { return reinterpret_cast<char*>(&_data[_curr]); }

    inline const char* c_rbuf() const { return reinterpret_cast<const char*>(&_data[_off]); }
    inline const char* c_wbuf() const { return reinterpret_cast<const char*>(&_data[_curr]); }

    BufferReader& read(void* v, size_type c, size_type* got = NULL)
    {
        if (!rsize())
            return *this;

        if (rsize() >= c)
        {
            MEMCPY(v, &_data[_off], c);
            _off += c;
            if (got)
                *got = c;
        }
        else
        {
            MEMCPY(v, &_data[_off], rsize());
            _off = _curr;
            if (got)
                *got = _curr - _off;
        }
        return *this;
    }

    template <typename U> BufferReader& read(U& v, bool* got = NULL)
    {
        if (rsize() >= sizeof(v))
        {
            v = *reinterpret_cast<U*>(&_data[_off]);
            _off += sizeof(v);
            if (got)
                *got = true;
        }
        return *this;
    }

    template <typename U>
    inline BufferReader& operator>>(U& val)
    {
        return read(val);
    }

    inline BufferReader& operator>>(std::string& str)
    {
        strlen_t len = 0;
        read(len);
        if (len)
        {
            str.resize(len);
            read(&str[0], len);
        }
        return *this;
    }

    inline const value_type& at(size_type i) const
    {
        if (i+_off >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
#if defined(CXX0X) && CXX0X
    template <typename U = value_type>
#else
    template <typename U>
#endif
    inline const U& at(size_type i = 0) const
    {
        if (i+_off >= _max || (wsize() < sizeof(U))) // XXX:
        {
            static U u;
            return u;
        }
        return *reinterpret_cast<U*>(&_data[i+_off]);
    }

    inline const value_type& operator[](size_type i) const
    {
        if (i+_off >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }
};

template <typename T = BufferContainer>
class BufferWriter : public BufferReader<T>
{
public:
    typedef BufferReader<T> Base;

    typedef typename T::size_type size_type;
    typedef typename T::value_type value_type;
    typedef typename T::const_pointer const_pointer;
    typedef typename T::pointer pointer;

public:
    using T::_data;
    using T::_off;
    using T::_curr;
    using T::_max;

    using T::rsize;
    using T::wsize;
    using T::empty;
    using T::clear;
    using T::checksize;

    using Base::rbuf;
    using Base::wbuf;
    using Base::operator>>;


public:
    BufferWriter(size_type size, size_type curr = 0, size_type off = 0)
        : BufferReader<T>(size, curr, off) {}

    inline BufferWriter& operator=(const BufferWriter& buf)
    {
        if (this == &buf)
            return *this;
        clear();
        return append(buf.rbuf(), buf.rsize());
    }

    template <typename U>
    inline BufferWriter& operator<<(const U& val)
    {
        if (!checksize(sizeof(val)))
            return *this;
        *reinterpret_cast<U*>(&_data[_curr]) = val;
        _curr += sizeof(val);
        return *this;
    }

    // XXX: save as [length][str]
    inline BufferWriter& operator<<(const char* str)
    {
        if (!str)
            return *this;
        strlen_t len = strlen(str);
        return append(str, len, BUF_LEN);
    }

    inline BufferWriter& operator<<(const std::string& str)
    {
        return (*this)<<(str.c_str());
    }

    inline BufferWriter& operator<<(const BufferWriter& buf)
    {
        if (this == &buf)
            return *this;
        return append(buf.rbuf(), buf.rsize()>wsize()?wsize():buf.rsize());
    }

    BufferWriter& operator>>(BufferWriter& buf)
    {
        if (this == &buf)
            return *this;
        if (rsize() > buf.wsize())
        {
            size_type c = rsize() - buf.wsize();
            buf.append(rbuf(), c);
            rflip(c);
        }
        else
        {
            buf.append(rbuf(), rsize());
            clear();
        }
        return *this;
    }

    inline BufferWriter& put(const void* buf, size_type size, unsigned char lennull = 0)
    {
        return append(buf, size, lennull);
    }

    // XXX: do not call remove() frequently
    void remove(size_t s, size_t n)
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
        if (i+_off >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
#if defined(CXX0X) && CXX0X
    template <typename U = value_type>
#else
    template <typename U>
#endif
    inline U& at(size_type i = 0)
    {
        if (i+_off >= _max || (wsize() < sizeof(U))) // XXX:
        {
            static U u;
            return u;
        }
        return *reinterpret_cast<U*>(&_data[i+_off]);
    }

    // XXX: DANGEROUS!!! YOU MUST KNOW WHAT YOR ARE DOING
    inline value_type& operator[](size_type i)
    {
        if (i+_off >= _max)
        {
            static value_type t;
            return t;
        }
        return *&_data[i+_off];
    }

    // XXX: wflip in BufferBase
    void rflip(size_type size = 0)
    {
        _off += size;
        if (_off < _curr)
        {
            if (_off)
            {
                size_type c = _curr - _off;
                MEMCPY(&_data[0], &_data[_off], c);
                _off = 0;
                _curr = c;
            }
        }
        else
        {
            _off = _curr = 0;
        }
    }

    BufferWriter& append(const void* buf, size_type size, unsigned char lennull = 0)
    {
        if (lennull)
        {
            size_type nsize = size;
            if (lennull & BUF_LEN) // XXX: size must be less or equal to 0xffff
            {
                if (size > 0xffff)
                    return *this;
                nsize += sizeof(strlen_t);
            }
            if (lennull & BUF_NULL)
                nsize += 1;
            if (!checksize(nsize))
                return *this;

            if (lennull & BUF_LEN)
            {
                // XXX: strlen_t to store size
                if (lennull & BUF_NULL)
                    *reinterpret_cast<strlen_t*>(&_data[_curr]) = size + 1;
                else
                    *reinterpret_cast<strlen_t*>(&_data[_curr]) = size;
                _curr += sizeof(strlen_t);
                MEMCPY(&_data[_curr], buf, size);
                _curr += size;
                if (lennull & BUF_NULL)
                    _data[_curr++] = '\0';
            }
            else if (lennull & BUF_NULL)
            {
                MEMCPY(&_data[_curr], buf, size);
                _curr += size;
                _data[_curr++] = '\0';
            }
        }
        else
        {
            if (!checksize(size))
                return *this;
            MEMCPY(&_data[_curr], buf, size);
            _curr += size;
        }
        return *this;
    }
};

class Buffer : public BufferWriter<BufferContainer>
{
public:
    typedef BufferWriter<BufferContainer> Base;

public:
    Buffer(size_type SIZE = DEFAULT_BUFFER_SIZE)
        : Base(SIZE)
    {}

    Buffer(const Buffer& b)
        : Base(b._max, b._curr, b._off)
    {
        MEMCPY(&_data[0], &b._data[0], _curr);
        rflip();
    }

    Buffer(const void* data, size_type size)
        : Base(DEFAULT_BUFFER_SIZE>size?DEFAULT_BUFFER_SIZE:size, size)
    {
        MEMCPY(&_data[0], data, _curr);
    }


    ~Buffer()
    {
        clear();
    }
};

template<size_t SIZE = TRUNK_SIZE>
class StackBuffer : public BufferWriter<BufferStack<SIZE> >
{
public:
    typedef BufferWriter<BufferStack<SIZE> > Base;
    typedef typename Base::size_type size_type;

public:
    using Base::rsize;
    using Base::wsize;

public:

    StackBuffer() : Base(SIZE)
    {}

    StackBuffer(const StackBuffer& sb) : Base(SIZE)
    {
        append(sb._data, sb.rsize()>wsize()?wsize():sb.rsize());
    }

    StackBuffer(const void* data, size_type size) : Base(SIZE)
    {
        append(data, size>wsize()?wsize():size);
    }

    ~StackBuffer()
    {}
};

class MemReader : public BufferReader<BufferMem>
{
public:
    typedef BufferReader<BufferMem> Base;

public:
    MemReader(const_pointer data, size_type size)
        : Base(size, size)
    {
        _data = const_cast<pointer>(data);
    }

    MemReader(const void* data, size_type size)
        : Base(size, size)
    {
        _data = reinterpret_cast<pointer>(const_cast<void*>(data));
    }

    ~MemReader()
    {}
};

} // namespace buf

#endif // BUFFER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

