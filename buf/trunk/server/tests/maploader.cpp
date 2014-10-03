
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Buffer.h"
#include "core/File.h"
#include "common/MapBlock.h"
#include "common/MapLoader.h"
#include "common/MapGrid.h"
using namespace buf;

void test1()
{
    Buffer b;
    b << 'B' << 'U' << 'F' << 'M';
    b << (u32_t)2;
    b << (u32_t)2;

    MapBlock mg;
    mg.flags = 0;
    mg.type = 0;

    b << mg;
    b << mg;
    b << mg;
    b << mg;

    File f("./mm");
    f.open(FILE_RW);
    f.write(b.rbuf(), b.rsize());
}

void test2()
{
    MapLoader<MapBlock> ml("./mm");
    ml.load();
}

int main(int argc, char* argv[])
{
    //test1();
    test2();
	return 0;
}
