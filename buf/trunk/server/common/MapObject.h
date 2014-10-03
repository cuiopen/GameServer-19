
#ifndef MAPOBJECT_H_
#define MAPOBJECT_H_

#include "GameObject.h"
#include "GridIndex.h"
#include "GUID.h"

namespace buf
{

class MapObject : public GameObject<GUID>
{
public:
    static const int ObjectType_Max = 16;

public:
    MapObject() : _grid(GRID_INVALID), _inWorld(false), _hide(false)
    {}

    MapObject(const GUID& id, int type, char* name) : GameObject<GUID>(id, type, name)
    {}

    virtual ~MapObject()
    {}

    inline const Pos& getPos() const { return _pos; }
    bool setPos(const Pos& mapWH, const Pos& p);

    inline PosI getGrid() const { return _grid; }
    inline void setGrid(PosI grid) { _grid = grid; }

    inline bool isInWorld() const { return _inWorld; }
    inline void setInWorld(bool v) { _inWorld = v; }

    inline bool isHide() const { return _hide; }
    inline void setHide(bool hide) { _hide = hide; }

protected:
    Pos _pos;
    PosI _grid;
    bool _inWorld;
    bool _hide;
};

} // namespace buf

#endif // MAPOBJECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

