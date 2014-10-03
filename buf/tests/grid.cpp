
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/RTime.h"
#undef USING_GRIDINDEX_CELL
#define USING_GRIDINDEX_NINE 1
#include "common/GridIndex_Nine.h"
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

void test1()
{
    GridIndex<Test> gi;

    setGridWH(Pos(14, 19));
    //printf("width: %u\n", GRID_WIDTH);
    GRID_WIDTH = 19;
    //printf("width: %u\n", GRID_WIDTH);

    gi.init(Pos(100,100));
    PosI pi;
    posToGrid(gi.getMapWH(), Pos(1,1), pi);

#if 0
    {
        //TimeFilm tf;
        const PosIVec_t& piv = gi.getAssociateGrid(0);
        for (size_t i = 0; i < piv.size(); ++i)
            printf("%u ", piv[i]);
    }
#endif

    Test a;
    Pos p(100, 100);
    gi.refresh(&a, p);
}

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

void test2()
{
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
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    test1();
    //test2();

	return 0;
}

