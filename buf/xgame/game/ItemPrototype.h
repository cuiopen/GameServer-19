
#ifndef ITEMPROTOTYPE_H_
#define ITEMPROTOTYPE_H_

namespace buf
{

enum ItemClass
{
    ITEM_CLASS_CONSUMABLE   = 0, // 消耗品
    ITEM_CLASS_WEAPON       = 1, // 武器
    ITEM_CLASS_ARMOR        = 2, // 防具
    ITEM_CLASS_GEM          = 3, // 宝石
    ITEM_CLASS_QUEST        = 4, // 任务
    ITEM_CLASS_CONTAINER    = 5, // 容器
    ITEM_CLASS_GENERIC      = 6, // 普通
    ITEM_CLASS_MAX
};

enum ItemSubClassConsumable
{
    ITEM_SUBCLASS_CONSUMABLE = 0,
};

class ItemPrototype
{
public:
    typedef ItemID_t key_type;
    bool init();

private:

};

static ItemClass_t getClass(ItemPrototype* ip)
{
    return 0;
}

static ItemSubclass_t getSubclass(ItemPrototype* ip)
{
    return 0;
}

} // namespace buf

#endif // ITEMPROTOTYPE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

