
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/RTime.h"
#undef USING_GRIDINDEX_NINE
#define USING_GRIDINDEX_CELL 1
#include "common/GridIndex.h"
#include "common/MapObject.h"
using namespace buf;

class Test
{
public:
    static const int ObjectType_Max = 12;

public:
    Test() : _type(0) {}
    int getType() const { return _type; }
    void setType(int type) { _type = type; }
    bool setPos(const Pos& mapWH, const Pos& p)
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
    const Pos& getPos() const { return _pos; }
    PosI getGrid() const { return _grid; }
    void setGrid(PosI grid) { _grid = grid; }
    bool isInWorld() const { return false; }
    void setInWorld(bool v) { _inWorld = v; }

private:
    Pos _pos;
    PosI _grid;
    int _type;
    bool _inWorld;
};

typedef MapObject A;

class E : public Enumerator<A>
{
public:
    bool operator()(A* ptr)
    {
        printf("type: %u, posi: %u, pos(%u,%u)\n", ptr->getType(), ptr->getGrid(), ptr->getPos().x, ptr->getPos().y);
        return true;
    }
};

void test1()
{
    GridIndex<Test> gif;
    //setGridWH(Pos(5, 5));
    setGridWH(Pos(14, 19));
    GRID_WIDTH = 19;
    setGridRadius(1);
    getDistance(Pos(1,1), Pos(100,9));
    gif.init(Pos(100,100));

    const PosIVec_t& v = gif.getAssociateGrid(20);
    printvec(v);
    printf("dir: %d\n", gif.getGridDirect(1,100));
    printf("0,0 - 10,10: %d\n", posInRange(Pos(0,0), Pos(10,10), 10));
    printf("0,0 - 10,11: %d\n", posInRange(Pos(0,0), Pos(10,11), 10));
    printf("5,5 - 9,8: %d\n", posInRange(Pos(5,5), Pos(9,8), 10));
    printf("0,0 & 3,3: %d\n", gif.isInOneGrid(Pos(0,0), Pos(3,3)));
    printf("0,0 & 9,9: %d\n", gif.isInOneGrid(Pos(0,0), Pos(9,9)));
}

void test2()
{
    setGridWH(Pos(5, 5));
    setGridRadius(2);

    GridIndex<A> gi;
    gi.init(Pos(1000,1000));

    A a;
    a.setType(1);
    gi.refresh(&a, Pos(1,1));
    A a1;
    a1.setType(1);
    gi.refresh(&a1, Pos(100,1));
    A a2;
    a2.setType(2);
    gi.refresh(&a2, Pos(100,50));
    A a3;
    a3.setType(3);
    gi.refresh(&a3, Pos(100,100));

    E e;
    printf("all type:\n");
    gi.enumerate(e);
    printf("type 0:\n");
    gi.enumerate(e, 0);
    printf("type 1:\n");
    gi.enumerate(e, 1);
    printf("type 2:\n");
    gi.enumerate(e, 2);
    printf("type 3:\n");
    gi.enumerate(e, 3);

    printf("enumerate by grid:\n");
    gi.enumerate(e, a3.getGrid());
}

void test3()
{
    setGridWH(Pos(5, 5));
    setGridRadius(2);

    GridIndex<A> gi;
    gi.init(Pos(100,100));

    PosIVec_t piv;
    gi.getGridByRange(100, 2, piv);
    printvec(piv);

    piv.clear();
    gi.getGridByRange(Pos(10,10), 2, piv);
    printvec(piv);
}
    
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    //test1();
    test2();
    //test3();
	return 0;
}

