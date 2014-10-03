
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/Loot.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Loot loot;
    std::vector<ItemInfo_t> items;

    printf("sizeof(Loot): %lu\n", sizeof(Loot));

    printf("同时概率产出指定个数\n");
    loot.init(1, "1,10000|2,10000|3,5000|4,3000,100");
    LOOT.add(1,loot);

    //loot.roll(items);
    for (size_t i = 0; i < items.size(); ++i)
        printf("items[%lu]: (%u,%u)\n", i, items[i].id, items[i].count);

    items.clear();
    printf("定概率产出指定个数\n");
    loot.init(2, "5,1000|6,2000,99|7,3000|8,4000,250");
    LOOT.add(2,loot);
    //loot.roll(items);
    for (size_t i = 0; i < items.size(); ++i)
        printf("items[%lu]: (%u,%u)\n", i, items[i].id, items[i].count);

    loot.init(2, "9,1000,22|10,5000,9|11,1000|12,3000,2");
    LOOT.add(3,loot);

    printf("同时概率产出各包\n");
    items.clear();
    loot.init(3, "1,10000|2,5000|3,10000");
    LOOT.add(4,loot);
    loot.roll(items);
    for (size_t i = 0; i < items.size(); ++i)
        printf("items[%lu]: (%u,%u)\n", i, items[i].id, items[i].count);

#if 0
    printf("包掉落\n");
    items.clear();
    loot.init(4, "1,3000|2,1000|3,6000");
    loot.roll(items);
    for (size_t i = 0; i < items.size(); ++i)
        printf("items[%lu]: (%u,%u)\n", i, items[i].id, items[i].count);

    printf("混合掉落\n");
    items.clear();
    loot.init(5, "1,1,1000|100,0,3000,10000|2,1,3000|3,1,1000|999,0,2000,9999");
    loot.roll(items);
    for (size_t i = 0; i < items.size(); ++i)
        printf("items[%lu]: (%u,%u)\n", i, items[i].id, items[i].count);
#endif

	return 0;
}
