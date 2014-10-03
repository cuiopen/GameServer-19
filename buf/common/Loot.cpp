
#include "Loot.h"
#include "core/Tokenizer.h"
#include "core/Random.h"

namespace buf
{

#ifndef LOOT_CHANCE_BASE
#define LOOT_CHANCE_BASE 10000
#endif

static bool initAll(Loot* loot, const char* lcfg);
static bool initChance(Loot* loot, const char* lcfg);
static bool initAllPkg(Loot* loot, const char* lcfg);
static bool initChancePkg(Loot* loot, const char* lcfg);
static bool initMix(Loot* loot, const char* lcfg);

static bool rollAll(const Loot* loot, std::vector<ItemInfo_t>& items);
static bool rollChance(const Loot* loot, std::vector<ItemInfo_t>& items);
static bool rollAllPkg(const Loot* loot, std::vector<ItemInfo_t>& items);
static bool rollChancePkg(const Loot* loot, std::vector<ItemInfo_t>& items);
static bool rollMix(const Loot* loot, std::vector<ItemInfo_t>& items);

bool Loot::init(u8_t type, const char* lcfg)
{
    if (!lcfg || !strlen(lcfg))
        return false;
    items.clear();
    if (type == LT_ALL)
        return initAll(this, lcfg);
    if (type == LT_CHANCE)
        return initChance(this, lcfg);
    if (type == LT_ALL_PKG)
        return initAllPkg(this, lcfg);
    if (type == LT_CHANCE_PKG)
        return initChancePkg(this, lcfg);
    if (type == LT_MIX)
        return initMix(this, lcfg);
    return false;
}

bool Loot::roll(std::vector<ItemInfo_t>& items)
{
    if (type == LT_ALL)
        return rollAll(this, items);
    if (type == LT_CHANCE)
        return rollChance(this, items);
    if (type == LT_ALL_PKG)
        return rollAllPkg(this, items);
    if (type == LT_CHANCE_PKG)
        return rollChancePkg(this, items);
    if (type == LT_MIX)
        return rollMix(this, items);

    return false;
}

bool initAll(Loot* loot, const char* lcfg)
{
    Tokenizer cfg(lcfg, '|');
    if (!cfg.size())
        return false;

    bool shot = false;
    for (size_t i = 0; i < cfg.size(); ++i)
    {
        Tokenizer item(cfg[i], ',');
        if (item.size() < 2)
            continue;
            
        LootItem li;
        li.iid = atoi(item[0].c_str());
        if (!li.iid)
            continue;
        li.chance = atoi(item[1].c_str());
        if (item.size() > 2)
            li.count = atoi(item[2].c_str());
        // XXX: init in constructor
        //else
        //    li.count = 1;

        loot->items.push_back(li);
        shot = true;
    }

    if (shot)
        loot->type = LT_ALL;
    return shot;
}

bool initChance(Loot* loot, const char* lcfg)
{
    Tokenizer cfg(lcfg, '|');
    if (!cfg.size())
        return false;

    bool shot = false;
    for (size_t i = 0; i < cfg.size(); ++i)
    {
        Tokenizer item(cfg[i], ',');
        if (item.size() < 2)
            continue;

        LootItem li;
        li.iid = atoi(item[0].c_str());
        if (!li.lpid)
            continue;
        li.chance = atoi(item[1].c_str());
        if (item.size() > 2)
            li.count = atoi(item[2].c_str());

        loot->items.push_back(li);
        shot = true;
    }

    if (shot)
        loot->type = LT_CHANCE;
    return shot;
}

bool initAllPkg(Loot* loot, const char* lcfg)
{
    Tokenizer cfg(lcfg, '|');
    if (!cfg.size())
        return false;

    bool shot = false;
    for (size_t i = 0; i < cfg.size(); ++i)
    {
        Tokenizer item(cfg[i], ',');
        if (item.size() < 2)
            continue;

        LootItem li;
        li.lpid = atoi(item[0].c_str());
        li.chance = atoi(item[1].c_str());

        loot->items.push_back(li);
        shot = true;
    }

    if (shot)
        loot->type = LT_ALL_PKG;
    return shot;
}

bool initChancePkg(Loot* loot, const char* lcfg)
{
    Tokenizer cfg(lcfg, '|');
    if (!cfg.size())
        return false;

    bool shot = false;
    for (size_t i = 0; i < cfg.size(); ++i)
    {
        Tokenizer item(cfg[i], ',');
        if (item.size() < 2)
            continue;

        LootItem li;
        li.lpid = atoi(item[0].c_str());
        li.chance = atoi(item[1].c_str());

        loot->items.push_back(li);
        shot = true;
    }

    if (shot)
        loot->type = LT_CHANCE_PKG;
    return shot;
}

bool initMix(Loot* loot, const char* lcfg)
{
    Tokenizer cfg(lcfg, '|');
    if (!cfg.size())
        return false;

    bool shot = false;
    for (size_t i = 0; i < cfg.size(); ++i)
    {
        Tokenizer item(cfg[i], ',');
        if (item.size() < 3)
            continue;

        LootItem li;
        li.lpid = atoi(item[0].c_str());
        li.isPack = atoi(item[1].c_str());
        li.chance = atoi(item[2].c_str());
        if (!li.isPack && item.size() > 3)
            li.count = atoi(item[3].c_str());

        loot->items.push_back(li);
        shot = true;
    }

    if (shot)
        loot->type = LT_MIX;
    return shot;
}


bool rollAll(const Loot* loot, std::vector<ItemInfo_t>& items)
{
    if (!loot->items.size())
        return false;

    Random rnd;
    LootChance_t chance = rnd(LOOT_CHANCE_BASE);
    DEBUG("%s>>> chance: %u\n", __PRETTY_FUNCTION__, chance);
    ItemInfo_t ii;
    bool shot = false;
    for (size_t i = 0; i < loot->items.size(); ++i)
    {
        const LootItem& li = loot->items[i];
        if (chance < li.chance)
        {
            ii.id = li.iid;
            ii.count = li.count;
            items.push_back(ii);
            shot = true;
        }
    }

    return shot;
}

bool rollChance(const Loot* loot, std::vector<ItemInfo_t>& items)
{
    if (!loot->items.size())
        return false;

    Random rnd;
    LootChance_t chance = rnd(LOOT_CHANCE_BASE);
    DEBUG("%s>>> chance: %u\n", __PRETTY_FUNCTION__, chance);
    ItemInfo_t ii;
    for (size_t i = 0; i < loot->items.size(); ++i)
    {
        const LootItem& li = loot->items[i];
        if (chance < li.chance)
        {
            ii.id = li.iid;
            ii.count = li.count;
            items.push_back(ii);
            return true;
        }
        else
            chance -= li.chance;
    }

    return false;
}

bool rollAllPkg(const Loot* loot, std::vector<ItemInfo_t>& items)
{
    if (!loot->items.size())
        return false;

    Random rnd;
    LootChance_t chance = rnd(LOOT_CHANCE_BASE);
    DEBUG("%s>>> chance: %u\n", __PRETTY_FUNCTION__, chance);
    bool shot = false;
    for (size_t i = 0; i < loot->items.size(); ++i)
    {
        const LootItem& li = loot->items[i];
        if (chance < li.chance)
        {
            Loot* l = LOOT.get(li.lpid);
            if (!l)
                return shot;
            shot = l->roll(items);
        }
    }

    return shot;
}

bool rollChancePkg(const Loot* loot, std::vector<ItemInfo_t>& items)
{
    if (!loot->items.size())
        return false;

    Random rnd;
    LootChance_t chance = rnd(LOOT_CHANCE_BASE);
    DEBUG("%s>>> chance: %u\n", __PRETTY_FUNCTION__, chance);
    for (size_t i = 0; i < loot->items.size(); ++i)
    {
        const LootItem& li = loot->items[i];
        if (chance < li.chance)
        {
            Loot* l = LOOT.get(li.lpid);
            if (!l)
                return false;
            return l->roll(items);
        }
        else
            chance -= li.chance;
    }

    return false;
}

bool rollMix(const Loot* loot, std::vector<ItemInfo_t>& items)
{
    if (!loot->items.size())
        return false;

    Random rnd;
    LootChance_t chance = rnd(LOOT_CHANCE_BASE);
    DEBUG("%s>>> chance: %u\n", __PRETTY_FUNCTION__, chance);
    for (size_t i = 0; i < loot->items.size(); ++i)
    {
        const LootItem& li = loot->items[i];
        if (chance < li.chance)
        {
            if (li.isPack)
            {
                Loot* l = LOOT.get(li.lpid);
                if (!l)
                    return false;
                return l->roll(items);
            }
            else
            {
                ItemInfo_t ii;
                ii.id = li.iid;
                ii.count = li.count;
                items.push_back(ii);
                return true;
            }
        }
        else
            chance -= li.chance;
    }

    return false;
}

bool LootMgr::add(const LootPack_t& lpid, const Loot& loot)
{
    if (_loots.find(lpid) != _loots.end())
        return false;
    _loots[lpid] = loot;
    return true;
}

Loot* LootMgr::get(const LootPack_t& lpid)
{
    iterator i = _loots.find(lpid);
    if (i != _loots.end())
        return &i->second;
    return 0;
}


} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

