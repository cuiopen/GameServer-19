
#ifndef LOOT_H_
#define LOOT_H_

#include "core/Config.h"
#include "core/UnorderedContainer.h"
#include "core/Singleton.h"
#include "ItemType.h" // XXX: for ItemID_t,ItemCount_t,ItemInfo_t
#include <vector>

namespace buf
{

/**
 * 概率基础值为：10000
 *
 * 1 - id1,chance1[,num1]|id2,chance2[,num2]|...|idN,chanceN[,numN] 同时概率产出指定个数，各跑各的chance
 * 2 - id1,chance1[,num1]|id2,chance2[,num2]|...|idN,chanceN[,numN] 指定概率产出指定个数，chance和为10000
 * 3 - pkg1,chance1|pkg2,chance2|...|pkgN,chanceN                   同时概率产出各包，各跑各的chance
 * 4 - pkg1,chance1|pkg2,chance2|...|pkgN,chanceN                   包掉落，chance和为10000
 * 5 - id1,ispack1,chance1[,num1]|id2,ispack2,chance2[,num2],...    混合掉落
 *
 **/

enum LootType
{
    LT_ALL          = 1,
    LT_CHANCE       = 2,
    LT_ALL_PKG      = 3,
    LT_CHANCE_PKG   = 4,
    LT_MIX          = 5,
    LT_MAX
};

typedef u16_t LootPack_t; // XXX: sizeof(LootPack_t) >= sizeof(ItemCount_t)
typedef u16_t LootChance_t;

struct LootItem
{
    LootItem()
        : isPack(false), chance(0), lpid(0), count(1) {}

    bool isPack;
    LootChance_t chance;
    union
    {
        ItemID_t iid;
        LootPack_t lpid;
    };
    ItemCount_t count;
};

struct Loot
{
    u8_t type;
    std::vector<LootItem> items;

    bool init(u8_t type, const char* lcfg);
    bool roll(std::vector<ItemInfo_t>& items);
};

class LootMgr : public Singleton<LootMgr>
{
public:
    typedef UNORDERED_MAP<LootPack_t, Loot> container;
    typedef container::iterator iterator;

public:
    bool add(const LootPack_t& id, const Loot& loot);
    Loot* get(const LootPack_t& lpid);

private:
    container _loots;
};

#define LOOT LootMgr::getInstance()

} // namespace buf

#endif // LOOT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

