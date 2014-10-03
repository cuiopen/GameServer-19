
#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "Config.h"
#include "Object.h"
#include "RWLock.h"
#include "Foreach.h"
#include "Enumerator.h"

#if defined(CXX0X) && CXX0X
#include <unordered_map>
#include <algorithm>
#else
#include <map>
#endif

namespace buf
{

template <typename V = Object<>, typename K = typename V::key_type>
class ObjectManager
{
public:
    typedef K key_type;
    typedef V* pointer;
    typedef const V* const_pointer;
#if defined(CXX0X) && CXX0X
    typedef std::unordered_map<key_type, V*> container;
#else
    typedef std::map<key_type, V*> container;
#endif
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;
    typedef typename container::size_type size_type;
    typedef typename container::value_type value_type;

public:
    ObjectManager() {}
    ~ObjectManager() { destory(); }

    void destory()
    {
        ScopeRWLock lock(_lock);
#if defined(CXX0X) && CXX0X
        for (auto s = _data.begin(), e = _data.end(); s != e; ++s)
            DELETE(s->second);
#else
        for (iterator s = _data.begin(), e = _data.end(); s != e; ++s)
            DELETE(s->second);
#endif
        _data.clear();
    }

    bool add(const key_type& id, const V* obj)
    {
        if (!obj)
            return false;
        ScopeRWLock lock(_lock);
        return _data.insert(std::make_pair(id, const_cast<V*>(obj))).second;
    }

    bool del(const key_type& id, bool release = true)
    {
        ScopeRWLock lock(_lock);
        iterator i = _data.find(id);
        if (i != _data.end())
        {
            if (release)
                DELETE(i->second);
            _data.erase(i);
            return true;
        }
        return false;
    }

    inline bool delById(const key_type& id, bool release = true)
    {
        return del(id, release);
    }

    const V* operator[](const key_type& id) const
    {
        ScopeRDLock lock(_lock);
        const_iterator i = _data.find(id);
        if (i != _data.end())
            return *&i->second;
        return 0;
    }

    V* operator[](const key_type& id)
    {
        ScopeRDLock lock(_lock);
        const_iterator i = _data.find(id);
        if (i != _data.end())
            return *&i->second;
        return 0;
    }

    inline const V* byId(const key_type& id) const { return operator[](id); }
    inline V* byId(const key_type& id) { return operator[](id); }

    inline bool empty() const { return _data.empty(); }
    inline size_type size() const { return _data.size(); }

    bool hasObject(const key_type& id) const
    {
        ScopeRDLock lock(_lock);
        const_iterator i = _data.find(id);
        return i != _data.end();
    }

    void foreach(Enumerator<value_type>& enumator)
    {
        ScopeRWLock lock(_lock);
        buf::foreach(_data.begin(), _data.end(), enumator);
    }

    void enumerate(Enumerator<value_type>& vistor)
    {
        ScopeRWLock lock(_lock);
        for (iterator i = _data.begin(); i != _data.end(); ++i)
        {
            if (!vistor(i->second))
                break;
        }
    }

    void clear()
    {
        ScopeRWLock lock(_lock);
        _data.clear();
    }

private:
    mutable RWLock _lock;
    container _data;
};

// objects will be managered by id() and name()
template <typename V = ObjectN<>, typename K = typename V::key_type>
class ObjectManagerN : public ObjectManager<V, K>
{
public:
    typedef std::string key_type;
    typedef ObjectManager<V, K> base;
    typedef ObjectManager<V, key_type> container;

public:
    ~ObjectManagerN()
    {
        _data.clear();
    }

    bool add(const V* obj)
    {
        if (!obj)
            return false;
        if (base::add(obj->id(), obj))
            return _data.add(obj->name(), obj);
        return false;
    }

    bool del(const V* obj)
    {
        if (!obj)
            return false;

        if (_data.del(obj->name(), false))
            return base::del(obj->id());
        return false;
    }

    inline bool delById(const K& id) { return del(byId(id)); }
    inline bool delByName(const key_type& name) { return del(_data[name]); }

    inline const V* byName(const key_type& name) const { return _data[name]; }
    inline V* byName(const key_type& name) { return _data[name]; }

private:
    container _data;
};

// objects will be managered by id() and tempid()
template <typename V = ObjectT<>,
         typename K = typename V::key_type,
         typename T = typename V::temp_type>
class ObjectManagerT : public ObjectManager<V, K>
{
public:
    typedef T key_type;
    typedef ObjectManager<V, K> base;
    typedef ObjectManager<V, key_type> container;

public:
    ~ObjectManagerT()
    {
        _data.clear();
    }

    bool add(const V* obj)
    {
        if (!obj)
            return false;
        if (base::add(obj->id()))
            return _data.add(obj->tempid(), obj);
        return false;
    }

    bool del(const V* obj)
    {
        if (!obj)
            return false;
        if (_data.del(obj->tempid(), false))
            return base::del(obj->id());
        return false;
    }

    inline bool delById(const K& id) { return del(byId(id)); }
    inline bool delByTempid(const key_type& tempid) { return del(_data[tempid]); }

    inline const V* byTempid(const key_type& tempid) const { return _data[tempid]; }
    inline V* byTempid(const key_type& tempid) { return _data[tempid]; }

private:
    container _data;
};

// objects will be managered by id() name(), and tempid()
template <typename V = ObjectNT<>,
         typename K = typename V::key_type,
         typename T = typename V::temp_type>
class ObjectManagerNT : public ObjectManagerN<V, K>
{
public:
    typedef T key_type;
    typedef ObjectManagerN<V, K> baseN;
    typedef ObjectManager<V, key_type> container;

public:
    ~ObjectManagerNT()
    {
        _data.clear();
    }

    bool add(const V* obj)
    {
        if (!obj)
            return false;
        if (baseN::add(obj))
            return _data.add(obj->tempid(), obj);
        return false;
    }

    bool del(const V* obj)
    {
        if (!obj)
            return false;
        if (_data.del(obj->tempid(), false))
            return baseN::del(obj);
        return false;
    }

    inline bool delById(const K& id) { return del(byId(id)); }
    inline bool delByTempid(const key_type& tempid) { return del(_data[tempid]); }
    using baseN::delByName;

    inline const V* byTempid(const key_type& tempid) const { return _data[tempid]; }
    inline V* byTempid(const key_type& tempid) { return _data[tempid]; }

private:
    container _data;
};

} // namespace buf

#endif // OBJECTMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

