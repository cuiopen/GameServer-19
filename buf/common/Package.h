
#ifndef PACKAGE_H_
#define PACKAGE_H_

#include "core/Noncopyable.h"
#include "core/Enumerator.h"

#include "GUID.h"
#include "Item.h"
#include "GameObjectMgr.h"

namespace buf
{

// XXX: ITEM must be drived from GameObject

template <typename ITEM>
class Package : public Noncopyable
{
public:
    typedef typename ITEM::id_type id_type;

public:
    Package(u16_t capacity, u16_t size, u16_t cur = 0)
        : _cur(cur), _size(size), _capacity(capacity)
    {
        if (_cur > _size)
            _cur = _size;
        if (_size > _capacity)
            _size = _capacity;
    }

    virtual ~Package() {}

    bool add(ITEM* i)
    {
        if (_cur >= _size)
            return false;

        if (_objs.push(i))
        {
            ++_cur;
            return true;
        }

        return false;
    }

    bool remove(ITEM* i)
    {
        if (!i || _cur)
            return false;
#if 0
        ITEM* ii = getItemById(i->getId());
        if (ii == i)
        {
            _objs.remove(i);
            --_cur;
            return true;
        }
        return false;
#else
        _objs.remove(i);
        --_cur;
        return true;
#endif
    }

    inline ITEM* getItemById(const id_type& id)
    {
        ITEM* i = 0;
        _objs.find(id, i);
        return i;
    }

    inline u16_t cur() const { return _size; }
    inline u16_t size() const { return _size; }
    inline u16_t space() const { return _size - _cur; }
    inline u16_t capacity() const { return _capacity; }

    inline void addSpace(u16_t size)
    {
        if (_size + size > _capacity)
            _size = _capacity;
        else
            _size += size;
    }

    inline void enumerate(Enumerator<ITEM>& e)
    {
        _objs.enumerate(e);
    }

private:
    u16_t _cur;
    u16_t _size;
    u16_t _capacity;
    GameObjectMgr<GOMgrById<id_type, ITEM*> > _objs;
};

} // namespace space

#endif // PACKAGE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

