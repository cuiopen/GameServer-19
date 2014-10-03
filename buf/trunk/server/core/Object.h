
#ifndef OBJECT_H_
#define OBJECT_H_

#include "Config.h"
#include "IDGenerator.h"
#include <string>

namespace buf
{

// FIXME: key_generator may be used in different threads

// XXX: only has id, id may be std::string
template <typename KG = IDGenerator >
class Object
{
public:
    typedef KG key_generator;
    typedef typename key_generator::key_type key_type;
public:
    Object(const key_type& id) : _id(id) {}
    virtual ~Object() {}
    inline const key_type& id() const { return _id; }
    inline key_type& id() { return _id; }
protected:
    key_type _id;
};

// XXX: has id but also name
template <typename KG = IDGenerator >
class ObjectN : public Object<KG>
{
public:
    typedef KG key_generator;
    typedef typename key_generator::key_type key_type;
    typedef std::string name_type;
public:
    ObjectN(const key_type& id, const name_type& name)
        : Object<key_generator>(id), _name(name) {}
    virtual ~ObjectN() {}
    inline const name_type& name() const { return _name; }
protected:
    name_type _name;
};

// XXX: has id and tempid
template <typename KG = IDGenerator, typename TG = IDGenerator >
class ObjectT : public Object<KG>
{
public:
    typedef KG key_generator;
    typedef TG temp_generator;
    typedef typename key_generator::key_type key_type;
    typedef typename temp_generator::key_type temp_type;
public:
    ObjectT(const key_type& id, const temp_type& tempid)
        : Object<key_generator>(id), _tempid(tempid) {}
    virtual ~ObjectT() {}
    inline const temp_type& tempid() const { return _tempid; }
    inline temp_type& tempid() { return _tempid; }
private:
    temp_type _tempid;
};

// XXX: has id,name and tempid
template <typename KG = IDGenerator, typename TG = IDGenerator >
class ObjectNT : public Object<KG>
{
public:
    typedef KG key_generator;
    typedef TG temp_generator;
    typedef typename key_generator::key_type key_type;
    typedef typename temp_generator::key_type temp_type;
    typedef std::string name_type;
public:
    ObjectNT(const key_type& id, const name_type& name, const temp_type& tempid)
        : Object<key_generator>(id), _tempid(tempid), _name(name) {}
    virtual ~ObjectNT() {}
    inline const name_type& name() const { return _name; }
    inline const temp_type& tempid() const { return _tempid; }
    inline temp_type& tempid() { return _tempid; }
private:
    temp_type _tempid;
    name_type _name;
};

// Create functions
#ifdef __APPLE__
template <typename T>
#else
template <typename T = Object<> >
#endif
static T* Create()
{
    typedef typename T::key_generator key_generator;
    typedef typename key_generator::key_type key_type;

    // XXX: different key_generator will create different key
    // XXX: return 0 when _KG is out of key
    static key_generator kg;
    key_type id = kg.get();
    if (id == kg.max())
        return 0;
    T* obj = BUFNEW T(id);
    return obj;
}

#ifdef __APPLE__
template <typename T>
#else
template <typename T = ObjectN<> >
#endif
static T* CreateN(const std::string& name)
{
    typedef typename T::key_generator key_generator;
    typedef typename key_generator::key_type key_type;
    typedef typename T::name_type name_type;

    static key_generator kg;
    key_type id = kg.get();
    if (id == kg.max())
        return 0;
    T* obj = BUFNEW T(id, name);
    return obj;
}

#ifdef __APPLE__
template <typename T>
#else
template <typename T = ObjectT<> >
#endif
static T* CreateT()
{
    typedef typename T::key_generator key_generator;
    typedef typename key_generator::key_type key_type;
    typedef typename T::temp_generator temp_generator;
    typedef typename temp_generator::key_type temp_type;

    static key_generator kg;
    static temp_generator tg;

    key_generator id = kg.get();
    if (id == kg.max())
        return 0;

    temp_generator tid = tg.get();
    if (tid == tg.max())
        return 0;

    T* obj = BUFNEW T(id, tid);
    return obj;
}

#ifdef __APPLE__
template <typename T>
#else
template <typename T = ObjectNT<> >
#endif
static T* CreateNT(const std::string& name)
{
    typedef typename T::key_generator key_generator;
    typedef typename key_generator::key_type key_type;
    typedef typename T::name_type name_type;
    typedef typename T::temp_generator temp_generator;
    typedef typename temp_generator::key_type temp_type;

    static key_generator kg;
    static temp_generator tg;

    key_type id = kg.get();
    if (id == kg.max())
        return 0;

    temp_type tid = tg.get();
    if (tid == tg.max())
        return 0;

    T* obj = BUFNEW T(id, name, tid);
    return obj;
}

} // namespace buf

#endif // OBJECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

