
#ifndef MAP_H_
#define MAP_H_

#include "MapObject.h"
#include "MapBlock.h" // XXX: be configurated
#include "MapGrid.h"

namespace buf
{

class Map : public GameObject<u32_t>, public MapGrid<MapBlock, MapObject>
{

public:
    Map(u32_t id, const char* name = NULL) : GameObject<u32_t>(id, name)
    {}

    virtual ~Map()
    {}

    bool addObject(MapObject* mo);
    bool removeObject(MapObject* mo);

    void broadcast(const void* buf, size_t size);
    void broadcastGrids(const Pos& pos, const void* buf, size_t size);
};

} // namespace buf

#endif // MAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

