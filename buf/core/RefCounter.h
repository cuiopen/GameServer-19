
#ifndef REFCOUNTER_H_
#define REFCOUNTER_H_

#include "Config.h"
#include "Atomic.h"

namespace buf
{

template <typename T>
class RefCounter
{
    template <typename U> friend class SharedPtr;
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

public:
    explicit RefCounter(const_pointer ptr)
        : _ptr(const_cast<pointer>(ptr)), _refcount(0) {}

    ~RefCounter()
    {
        if (_ptr)
            DELETENULL(_ptr);
    }

    inline void ref()
    {
        ++_refcount;
    }

    inline void deref()
    {
        --_refcount;
    }

    inline int references() { return (int)_refcount; }

private:
    pointer _ptr;
    Atomic _refcount;
};

} // namespace buf

#endif // REFCOUNTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

