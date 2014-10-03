
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "core/Config.h"
#include "core/Assertion.h"
#include "core/Utils.h"
#include "GUID.h"
#include <string>

namespace buf
{

template <typename ID = GUID>
class GameObjectI
{
public:
    typedef ID id_type;

public:
    GameObjectI() {}
    explicit GameObjectI(const id_type& id) : _id(id) {}

public:
    inline const ID& getId() const { return _id; }
    inline void setId(const ID& id) { _id = id; }

protected:
    ID _id;
};

template <typename ID = GUID>
class GameObjectIN : public GameObjectI<ID>
{
public:
    GameObjectIN() : GameObjectI<ID>()
    {
        MEMZRO(_name, sizeof(_name));
    }

    explicit GameObjectIN(const ID& id, const char* name = NULL)
        : GameObjectI<ID>(id)
    {
        cpystr(name, _name, sizeof(_name), "null");
    }

public:
    inline void setName(const char* name) { cpystr(name, _name, sizeof(_name), "null"); }
    inline const char* getName() const { return _name; }

protected:
    char _name[NAME_MAX+1];
};

template <typename ID = GUID>
class GameObject : public GameObjectIN<ID>
{
public:
    GameObject() : GameObjectIN<ID>()
    {}

    GameObject(ID id, const char* name, int type = 0) : GameObjectIN<ID>(id, name), _type(type)
    {}

    explicit GameObject(const ID& id, int type = 0, const char* name = NULL)
        : GameObjectIN<ID>(id, name), _type(type)
    {}

    virtual ~GameObject()
    {}

    inline int getType() const { return _type; }
    inline void setType(int v) { _type = v; }

protected:
    int _type;
};

} // namespace buf

#endif // GAMEOBJECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

