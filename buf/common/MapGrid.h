
#ifndef MAPGRID_H_
#define MAPGRID_H_

#include "Pos.h"
#include "GridIndex.h"
#include "MapLoader.h"

namespace buf
{

template <typename MB/*MapGrid*/, typename MO/*MapObject*/>
class MapGrid : public GridIndex<MO>
{
public:
    typedef std::vector<MB> container;

public:
    MapGrid() {}
    virtual ~MapGrid() {}

    virtual bool init(MapLoader<MB>& loader)
    {
        if (!loader.isLoaded() && !loader.load())
            return false;

        if (!loader.getData().rsize())
            return false;

        Buffer& data = loader.getData();
        _grids.resize(loader.size());
        MEMCPY(&_grids[0], data.rbuf(), data.rsize());

        GridIndex<MO>::init(Pos(loader.width(), loader.height()));
        return true;
    }

    inline bool isBlock(const Pos& pos, int flag)
    {
        if (GridIndex<MO>::posValidate(pos))
            return _grids[pos.y * GridIndex<MO>::getWidth() + pos.x].flags & flag;
        return false;
    }

    inline void clearBlock(const Pos& pos, int flag)
    {
        if (GridIndex<MO>::posValidate(pos))
            _grids[pos.y * GridIndex<MO>::getWidth() + pos.x].flags &= ~flag;
    }

    inline void setBlock(const Pos& pos, int flag)
    {
        if (GridIndex<MO>::posValidate(pos))
            _grids[pos.y * GridIndex<MO>::getWidth() + pos.x].flags |= flag;
    }

protected:
    container _grids;
};

} // namespace buf

#endif // MAPGRID_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

