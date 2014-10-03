
#ifndef SHAREDPTR_H_
#define SHAREDPTR_H_

#include "RefCounter.h"
#include "Assertion.h"

namespace buf
{

template <typename T>
class SharedPtr
{
    template <typename U> friend class SharedPtr;
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef RefCounter<T> counter;
    typedef counter* counter_pointer;

public:

    // XXX: Reference to a NULL(0)
    SharedPtr() : _ref(0)
    {
        _ref = BUFNEW counter(0);
        if (_ref)
            _ref->ref();
    }

    SharedPtr(const_pointer ptr) : _ref(0)
    {
        _ref = BUFNEW counter(ptr);
        if (_ref)
            _ref->ref();
    }

    SharedPtr(const SharedPtr& ptr) : _ref(ptr._ref)
    {
        if (_ref)
            _ref->ref();
    }

    template <typename U> SharedPtr(const SharedPtr<U>& ptr) : _ref(0)
    {
        typename SharedPtr<U>::pointer check_base = 0;
        pointer check_drived = check_base;(void)(check_drived);
        _ref = reinterpret_cast<counter_pointer>(ptr._ref);
        if (_ref)
            _ref->ref();
    }

    ~SharedPtr()
    {
        if (_ref)
            _ref->deref();
        if (!_ref->references())
            DELETENULL(_ref);
    }

    SharedPtr& operator=(const SharedPtr& ptr)
    {
        if (_ref)
        {
            if (get() == ptr.get())
                return *this;
            _ref->deref();
            if (!_ref->references())
                DELETENULL(_ref);
        }

        _ref = ptr._ref;
        if (_ref)
            _ref->ref();
        return *this;
    }

    template <typename U> SharedPtr& operator=(const SharedPtr<U>& ptr)
    {
        typename SharedPtr<U>::pointer check_base = 0;
        pointer check_drived = check_base;(void)(check_drived);

        if (_ref)
        {
            if (get() == ptr.get())
                return *this;
            _ref->deref();
            if (!_ref->references())
                DELETENULL(_ref);
        }

        _ref = reinterpret_cast<counter_pointer>(ptr._ref);
        if (_ref)
            _ref->ref();
        return *this;
    }

    inline pointer get()
    {
        if (_ref)
            return const_cast<pointer>(_ref->_ptr);
        return 0;
    }
    inline const_pointer get() const
    {
        if (_ref)
            return _ref->_ptr;
        return 0;
    }

    inline pointer operator->()
    {
        ASSERT(_ref);
        return get();
    }

    inline reference operator*()
    {
        ASSERT(_ref);
        return *get();
    }

    inline bool operator!() const { return !_ref && get(); }

    inline bool operator!=(const_pointer ptr) const
    {
        if (_ref)
            return ptr != _ref->_ptr;
        else
            return ptr || _ref->_ptr;
    }
    inline bool operator!=(const SharedPtr& ptr) const { return ptr.get() != get(); }

    template <typename U> inline bool operator!=(const SharedPtr<U>& ptr) const
    {
        typename SharedPtr<U>::pointer check_base = 0;
        pointer check_drived = check_base;
        return ptr.get() != get();
    }

    inline bool operator==(const_pointer ptr) const { return ! operator!=(ptr); }
    inline bool operator==(const SharedPtr& ptr) const { return ! operator!=(ptr); }
    template <typename U> inline bool operator==(const SharedPtr<U>& ptr) const
    { return ! operator!=(ptr); }

private:
    SharedPtr& operator= (const_pointer ptr);

private:
    counter* _ref;
};

} // namespace buf

#endif // SHAREDPTR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

