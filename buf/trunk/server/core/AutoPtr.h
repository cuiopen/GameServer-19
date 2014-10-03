
#ifndef AUTOPTR_H_
#define AUTOPTR_H_

#include "Config.h"
#include "Assertion.h"

namespace buf
{

template <typename T>
class AutoPtr
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

public:
    explicit AutoPtr(const_pointer ptr) : _ptr(ptr) {}
    explicit AutoPtr(pointer ptr) : _ptr(ptr) {}
    ~AutoPtr() { reset(); }

    inline reference operator*() { ASSERT(_ptr); return *_ptr; }
    inline pointer operator->() { ASSERT(_ptr); return _ptr; }

    inline bool operator!() { return !_ptr; }
    inline pointer get() const { return _ptr; }
    inline bool isnull() const { return !_ptr; }

    inline pointer release(void)
    {
        pointer old = _ptr;
        _ptr = 0;
        return old;
    }
    inline void reset(pointer p = 0)
    {
        if (_ptr != p)
            DELETENULL(_ptr);
        _ptr = p;
    }

private:
    AutoPtr(const AutoPtr& ptr);
    template <typename U> AutoPtr(const AutoPtr<U>& ptr);
    template <typename U> inline const AutoPtr<U>& operator=(const AutoPtr<U>& ptr);
    const AutoPtr& operator=(const AutoPtr& ptr);
    const AutoPtr& operator=(const_pointer ptr);

private:
    mutable pointer _ptr;
};

template <typename T>
class AutoArrPtr
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

public:
    explicit AutoArrPtr(const_pointer ptr) : _ptr(ptr) {}
    explicit AutoArrPtr(pointer ptr) : _ptr(ptr) {}
    ~AutoArrPtr() { reset(); }

    inline reference operator*() { ASSERT(_ptr); return *_ptr; }
    inline pointer operator->() { ASSERT(_ptr); return _ptr; }

    inline bool operator!() { return !_ptr; }
    inline pointer get() const { return _ptr; }
    inline bool isnull() const { return !_ptr; }

    inline pointer release(void)
    {
        pointer old = _ptr;
        _ptr = 0;
        return old;
    }
    inline void reset(pointer p = 0)
    {
        if (_ptr != p)
            DELETENULL(_ptr);
        _ptr = p;
    }

private:
    AutoArrPtr(const AutoArrPtr& ptr);
    template <typename U> AutoArrPtr(const AutoArrPtr<U>& ptr);
    template <typename U> inline const AutoArrPtr<U>& operator=(const AutoArrPtr<U>& ptr);
    const AutoArrPtr& operator=(const AutoArrPtr& ptr);
    const AutoArrPtr& operator=(const_pointer ptr);

private:
    mutable pointer _ptr;
};

} // namespace buf

#endif // AUTOPTR_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

