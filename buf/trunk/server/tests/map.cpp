
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/Map.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    Map map(1);
    MapLoader<MapBlock> ml("./mm");
    map.init(ml);

	return 0;
}
