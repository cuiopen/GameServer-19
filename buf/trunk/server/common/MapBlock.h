
#ifndef MAPBLOCK_H_
#define MAPBLOCK_H_

namespace buf
{

// TODO: 如果有不同的需求，则需要修改这个文件

// XXX: 如果有更多的阻挡类型需要在这里加
#define MAP_BLOCK_NONE      0x00 // 无阻挡
#define MAP_BLOCK_TILE      0x01 // 一般阻挡
#define MAP_BLOCK_WATER     0x02 // 水阻挡
#define MAP_BLOCK_MAGIC     0x04 // 魔法阻挡
#define MAP_BLOCK_FLY       0x08 // 飞行阻挡
#define MAP_BLOCK_ENTRY     0x10 // Player或者NPC阻挡
#define MAP_BLOCK_OBJECT    0x20 // 物品阻挡
#define MAP_BLOCK_RESERVED1 0x40
#define MAP_BLOCK_RESERVED2 0x80

#define MAP_BLOCK_ALL       (MAP_BLOCK_TILE|MAP_BLOCK_WATER|MAP_BLOCK_MAGIC|MAP_BLOCK_FLY)

#pragma pack(1)
typedef u8_t MapGridFlag_t;

struct MapBlock
{
    MapGridFlag_t flags;
    u8_t type;
};
#pragma pack()

} // namespace buf

#endif // MAPBLOCK_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
