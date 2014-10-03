
#ifndef GAMEOBJECTMGR_H_
#define GAMEOBJECTMGR_H_

#include "common/GameObject.h"
#include "core/UnorderedContainer.h" // XXX: for HASH_NAMESPACE_START, HASH_NAMESPACE_END
#include "core/Hash.h"
#include "core/Random.h"
#include "core/Enumerator.h"
#include "core/TypeTraits.h"

HASH_NAMESPACE_START

template<>
struct hash<buf::GUID>
{
    inline size_t operator()(const buf::GUID& k) const
    {
        return k.getRawValue();
    }
};

template<>
struct hash<const char*>
{
    inline size_t operator()(const char* k) const
    {
        return buf::sfhash(k);
    }
};

HASH_NAMESPACE_END

namespace buf
{

template <typename K>
struct key_equal : public std::binary_function<K, K, bool>
{
    inline bool operator()(const K k1, const K k2) const;
};

template<>
inline bool key_equal<u32_t>::operator()(const u32_t k1, const u32_t k2) const
{
    return k1 == k2;
}

template<>
inline bool key_equal<GUID>::operator()(const GUID k1, const GUID k2) const
{
    return k1 == k2;
}

template<>
inline bool key_equal<const char*>::operator()(const char* k1, const char* k2) const
{
    return strcmp(k1, k2) == 0;
}

template<typename K, typename V>
class GOHashMap
{
public:
    typedef UNORDERED_MAP<K, V, HASH_NAMESPACE::hash<K>, key_equal<K> > container;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

public:

    inline bool insert(const K& k, V& v)
    {
        _objs[k] = v;
        return true;
    }

    inline bool find(const K& k, V& v) const
    {
        const_iterator i = _objs.find(k);
        if (i != _objs.end())
        {
            v = i->second;
            return true;
        }
        return false;
    }

    inline void remove(const K& k) { _objs.erase(k); }
    inline void clear() { _objs.clear(); }
    inline bool empty() const { return empty(); }
    inline size_t size() const { return _objs.size(); }

protected:
    container _objs;
};

template<typename K, typename V>
class GOMultiHashMap
{
public:
    typedef UNORDERED_MULTIMAP<K, V, HASH_NAMESPACE::hash<K>, key_equal<K> > container;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

public:

    bool insert(const K& k, V& v)
    {
        _objs.insert(std::pair<K,V>(k,v));
        return true;
    }

    inline void remove(const K& k) { _objs.erase(k); }
    inline void clear() { _objs.clear(); }
    inline bool empty() const { return empty(); }
    inline size_t size() const { return _objs.size(); }

protected:
    container _objs;
};

// XXX: need V::getId()
template<typename K, typename V>
class GOMgrById : public GOHashMap<K, V>
{
public:
    typedef K id_type;
    typedef V value_type;

public:
    inline bool push(V& v)
    {
        V yav = 0;
        if (!find(v->getId(), yav))
        {
            insert(v->getId(), v);
            return true;
        }
        return false;
    }

    inline void remove(V& v)
    {
        GOHashMap<K, V>::remove(v->getId());
    }
};

// XXX: need V::getName()
template <typename V>
class GOMgrByName : public GOHashMap<const char*, V>
{
public:
    typedef const char* id_type;
    typedef V value_type;

public:
    inline bool push(V& v)
    {
        V yav = 0;
        if (!find(v->getName(), yav))
        {
            insert(v->getName(), v);
            return true;
        }
        return false;
    }

    inline void remove(V& v)
    {
        GOHashMap<const char*, V>::remove(v->getName());
    }
};

template <typename V>
class GOMgrByMultiName : public GOMultiHashMap<const char*, V>
{
public:
    typedef const char* id_type;
    typedef V value_type;

    typedef typename GOMultiHashMap<const char*, V>::iterator iterator;
    typedef typename GOMultiHashMap<const char*, V>::const_iterator const_iterator;
    typedef GOMultiHashMap<const char*, V> base;

public:
    inline bool push(V& v)
    {
        return insert(v->getName(), v);
    }

    inline void remove(V& v)
    {
        std::pair<iterator, iterator> its = base::_objs.equal_range(v->getName());
        for(iterator it = its.first; it != its.second; ++it)
        {
            if (it->second == v)
            {
                base::_objs.erase(it);
                return;
            }
        }
    }

    bool find(const char* name, V& v, bool r = false) const
    {
        if (!name)
            return false;

        int cnt = base::_objs.count(name);
        if(cnt > 0)
        {
            int mrd = 0;
            int j = 0;

            Random rnd;
            if (r && cnt > 1)
                mrd = rnd(cnt-1);

            std::pair<const_iterator,const_iterator> its = base::_objs.equal_range(name);
            for(const_iterator it = its.first; it != its.second && j < cnt; ++it, ++j)
            {
                if (mrd == j)
                {
                    v = it->second;
                    return true;
                }
            }
        }
        return false;
    }
};

template <typename K, typename V>
class GOMgrByNone
{
public:
    inline bool push(V&) { return true; }
    inline void remove(V&) {}
    inline void clear() {}
    inline bool find() const { return true; }
};

template <typename M1, typename M2 = GOMgrByNone<typename M1::id_type, typename M1::value_type> >
class GameObjectMgr : public M1, public M2
{
public:
    using M1::find;
    using M2::find;

    inline bool push(typename M1::value_type& v)
    {
        if (M1::push(v))
        {
            if (M2::push(v))
                return true;
            else
                M1::remove(v);
        }
        return false;
    }

    inline size_t size() const 
    {
        return M1::size();
    }

    inline bool remove(typename M1::value_type& v)
    {
        M2::remove(v);
        M1::remove(v);
        return true;
    }

    inline void clear()
    {
        M1::clear();
        M2::clear();
    }

    void enumerate(Enumerator<typename remove_pointer<typename M1::value_type>::type>& e)
    {
        for (typename M1::container::iterator i = M1::_objs.begin(); i != M1::_objs.end(); ++i)
        {
            if (!e(i->second))
                break;
        }
    }
};

} // namespace buf

#endif // GAMEOBJECTMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

