
#include "MapObject.h"

namespace buf
{

bool MapObject::setPos(const Pos& mapWH, const Pos& p)
{
    PosI grid = (PosI)-1;;
    if (posToGrid(mapWH, p, grid))
    {
        _grid = grid;
        _pos = p;
        return true;
    }
    return false;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

