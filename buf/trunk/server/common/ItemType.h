
#ifndef ITEMTYPE_H_
#define ITEMTYPE_H_

#include "core/Config.h"

namespace buf
{

typedef u16_t ItemID_t;
typedef u16_t ItemCount_t;
typedef u16_t ItemClass_t;
typedef u16_t ItemSubclass_t;

typedef enum ItemColor
{
    ITEM_COLOR_WHITE = 0,
    ITEM_COLOR_GREEN = 1,
    ITEM_COLOR_BLUE  = 2,
    ITEM_COLOR_PURPLE= 3,
    ITEM_COLOR_ORANGE= 4,
    ITEM_COLOR_GOLD  = 5,
    ITEM_COLOR_MAX
} ItemColor_e;

typedef struct ItemInfo
{
    ItemID_t id; // ItemPrototype ID
    ItemCount_t count;
} ItemInfo_t;

} // namespace buf

#endif // ITEMTYPE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

