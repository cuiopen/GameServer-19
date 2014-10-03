
#ifndef GRIDINDEX_NINE_H_
#define GRIDINDEX_NINE_H_

#include "core/UnorderedContainer.h"
#include "core/Enumerator.h"
#include "Pos.h"

#include <algorithm>

namespace buf
{

/**
 * 九屏对象管理系统
 *
 *  (0,0)
 *    --------------------------------> x
 *    |    
 *    |   [x][x][x]
 *    |   [x][1][2][3]
 *    |   [x][4][*][6][x]
 *    |      [7][8][9][x]
 *    |         [x][x][x]
 *    |
 *    |
 *    |    [x][1][2][3][x]
 *    |    [x][4][5][6][x]
 *    |    [x][7][8][9][x]
 *    |
 *    |
 *    V                                (w,h)
 *    y
 *
 **/

#define GRID_INVALID ((u32_t)-1)

extern u32_t GRID_WIDTH; // XXX: >= 3
extern u32_t GRID_HEIGHT; // XXX: >= 3

static void setGridWH(const Pos& gridWH)
{
    GRID_WIDTH  = gridWH.x;
    GRID_HEIGHT = gridWH.y;
}

static bool posToGrid(const Pos& mapWH, const Pos& p, PosI& pi)
{
    if (p.x < mapWH.x && p.y < mapWH.y)
    {
        pi = ((p.y/GRID_HEIGHT) * ((mapWH.x+GRID_WIDTH-1)/GRID_WIDTH)) + (p.x/GRID_WIDTH);
        return true;
    }
    return false;
}

// XXX:
// T must be drived from MapObject
//      need T::ObjectType_Max,
//
template <typename T>
class GridIndex
{
public:
    typedef UNORDERED_SET<T*> ObjectSet;
    typedef typename ObjectSet::iterator OSIterator;
    typedef UNORDERED_MAP<PosI, ObjectSet> ObjectContainer;
    typedef typename ObjectContainer::iterator OCIterator;

    typedef UNORDERED_MAP<PosI, PosIVec_t> GridContainer;
    typedef typename GridContainer::iterator GridIterator;
    typedef typename GridContainer::const_iterator ConstGridIterator;

public:

    GridIndex() {}
    virtual ~GridIndex() {}

    // XXX:
    //  mapWH - the max cell(x,y) of screen
    void init(const Pos& mapWH);

    const Pos getMapWH() const { return _mapWH; }
    // 取得第p屏相关联的九屏
    const PosIVec_t& getAssociateGrid(const PosI& p) const;

    bool refresh(T* obj, const Pos& p);
    void remove(T* obj);

    void enumerate(Enumerator<T>& e, int type = T::ObjectType_Max);

protected:
    // 整个地图的格子宽和高
    Pos _mapWH;
    // 横向屏幕(Grid)个数及纵向屏幕个数(每个屏里由多个格子组成)
    Pos _gridWH;
    // 最大屏幕索引
    PosI _idxMax;
    // 每个屏的相关屏
    GridContainer _gridsIdx;
    // 八个方向的正向差分
    GridContainer _enterGrids[8];
    // 八个方向的反向差分
    GridContainer _leaveGrids[8];
    // 各屏幕里的对象容器
    ObjectContainer _objsIdx[T::ObjectType_Max];
    // 所有对象容器
    ObjectSet _objs[T::ObjectType_Max];
};

inline void printvec(PosIVec_t& v)
{
    std::sort(v.begin(), v.end());
    for (size_t i = 0; i < v.size(); ++i)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

template <typename T>
void GridIndex<T>::init(const Pos& mapWH)
{
    _mapWH = mapWH;
    _gridWH.x = (mapWH.x + GRID_WIDTH - 1) / GRID_WIDTH;
    _gridWH.y = (mapWH.y + GRID_HEIGHT - 1) / GRID_HEIGHT;
    _idxMax = _gridWH.x * _gridWH.y;

    // 九屏调整
    const int offs[9][2] =
    {
        { 0, -1}, { 1, -1}, { 1, 0},
        { 1,  1}, { 0,  1}, {-1, 1},
        {-1,  0}, {-1, -1}, { 0, 0}
    };

    PosIVec_t piv;

    for (u32_t i = 0; i < _idxMax; ++i)
    {
        printf(">>");
        int gridX = i % _gridWH.x;
        int gridY = i / _gridWH.x;

        for(int j = 0; j < 9; ++j) {
            int x = gridX + offs[j][0];
            int y = gridY + offs[j][1];

            if (x >= 0 && y >= 0 && x < (int)_gridWH.x && y < (int)_gridWH.y)
            {
                piv.push_back(y * _gridWH.x + x);
            }
        }

        printvec(piv);

        _gridsIdx[i] = piv;
        piv.clear();

        for (int dir = 0; dir < 8; ++dir)
        {
            int start, end;

            if (dir % 2 == 1)
            {
                start = 6;
                end = 10;
            }
            else
            {
                start = 7;
                end = 9;
            }

            for (int j = start; j <= end; ++j)
            {
                int x = gridX + offs[(j+dir)%8][0];
                int y = gridY + offs[(j+dir)%8][1];

                if (x >= 0 && y >= 0 && x < (int)_gridWH.x && y < (int)_gridWH.y)
                    piv.push_back(y * _gridWH.x + x);
            }

            printf("切入: ");
            printvec(piv);

            _enterGrids[dir][i] = piv;
            piv.clear();
        }

        for (int dir = 0; dir < 8; ++dir)
        {
            int start, end;

            if (dir % 2 == 1)
            {
                start = 2;
                end = 6;
            }
            else
            {
                start = 3;
                end = 5;
            }

            for (int j = start; j <= end; ++j)
            {
                int x = gridX + offs[(j+dir)%8][0];
                int y = gridY + offs[(j+dir)%8][1];

                if (x >= 0 && y >= 0 && x < (int)_gridWH.x && y < (int)_gridWH.y)
                    piv.push_back(y * _gridWH.x + x);
            }

            printf("退出: ");
            printvec(piv);
            _leaveGrids[dir][i] = piv;
            piv.clear();
        }
    }
}

template <typename T>
const PosIVec_t& GridIndex<T>::getAssociateGrid(const PosI& p) const
{
    static PosIVec_t null;
    if (p < _idxMax)
    {
        ConstGridIterator i = _gridsIdx.find(p);
        if (i != _gridsIdx.end())
            return i->second;
    }
    return null;
}

template <typename T>
bool GridIndex<T>::refresh(T* obj, const Pos& p)
{
    if (!obj)
        return false;
    int type = obj->getType();
    if (type >= T::ObjectType_Max)
        return false;

    PosI pi;
    if (!posToGrid(getMapWH(), p, pi))
        return false;

    if (obj->isInWorld())
    {
        PosI old = obj->getGrid();
        if (obj->setPos(getMapWH(), p))
        {
            if (pi != old)
            {
                _objsIdx[type][old].erase(obj);
                return _objsIdx[type][pi].insert(obj).second;
            }
        }
    }
    else
    {
        if (obj->setPos(getMapWH(), p))
        {
            if (_objsIdx[type][pi].insert(obj).second)
            {
                if (_objs[type].insert(obj).second)
                {
                    obj->setInWorld(true);
                    return true;
                }
                else
                    _objsIdx[type][pi].erase(obj);
            }
        }
    }
    return false;
}

template <typename T>
void GridIndex<T>::remove(T* obj)
{
    if (!obj)
        return false;
    int type = obj->getType();
    if (type >= T::ObjectType_Max)
        return false;
    OSIterator i = _objs[type].find(obj);
    if (i == _objs[type].end())
        return false;

    _objsIdx[type][obj->getGrid()].erase(obj);
    _objs[type].erase(obj);
    obj->setInWorld(false);

    return true;
}

template <typename T>
void GridIndex<T>::enumerate(Enumerator<T>& e, int type)
{
    int start = 0;
    int end = 0;

    if (type < T::ObjectType_Max)
    {
        start = type;
        end = type+1;
    }
    else
    {
        end = T::ObjectType_Max;
    }

    for (int i = start; i < end; ++i)
    {
        for (OSIterator it = _objs[i].begin(); it != _objs[i].end(); ++it)
        {
            int t = (*it)->getType();
            if (type != T::ObjectType_Max && t != type)
                continue;
            if (!e(*it))
                return;
        }
    }
}

} // namespace buf

#endif // GRIDINDEX_NINE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

