
#ifndef GUID_H_
#define GUID_H_

#include "core/Config.h"
#include "core/Singleton.h"

namespace buf
{

/**
 * [1.5:zone]           zone id, [0,4095]
 * [0.5:gameserver]     generatored by which gameserver, [0,15] 0-player
 * [0.5:type]           type of id, [0,15]
 *                          0 - player,
 *                          1 - item
 *                          2 - equipment
 *                          3 - pet
 *                          4 - unit hero,fighter
 *                          5 - NPC (R)
 *                          6 - dynamicobject (R)
 *
 *                          11-mail
 *                          12-sale
 *                          13-trade
 *                          14-group
 *                          15-session
 * if have no role id
 *      [5.5:counter]   counter, [0,17592186044415]
 * else
 *      [0.5:roleid]    0-15 role id
 *      [5:counter]     counter, [0,1099511627775]
 * end
 *
 * id will be recycled into recycle_ids table when the owner release it
 **/

enum GUIDType
{
    GUID_PLAYER = 0,
    GUID_ITEM = 1,
    GUID_EQUIPMENT = 2,
    GUID_PET = 3,
    GUID_UNIT = 4,
    GUID_NPC = 5,
    GUID_DYNAMICOBJECT = 6, // XXX: start from 1

    GUID_MAIL = 11,
    GUID_SALE = 12,
    GUID_TRADE = 13,
    GUID_GROUP = 14,
    GUID_SESSION = 15,
    GUID_MAX = 16
};

typedef u64_t GUID_t;

class GUID
{
public:
    GUID() : _guid(0) {}
    explicit GUID(GUID_t guid) : _guid(guid) {}

    inline GUID_t getRawValue() const { return _guid; }
    inline void setRawValue(GUID_t guid) { _guid = guid; }
    inline void clear() { _guid = 0; }

    // XXX: you must modify follow to fit your system
    inline u16_t getZone() const { return ((0xFFF0000000000000LL&_guid)>>52); }
    inline void setZone(u16_t zone) { _guid &= ~0xFFF0000000000000LL; _guid |= ((zone&0xFFFLL)<<52); }

    inline u8_t getGameServer() const { return ((0x000F000000000000LL&_guid)>>48); }
    inline void setGameServer(u8_t server) { _guid &= ~0x000F000000000000LL; _guid |= ((server&0xFLL)<<48); }

    inline u8_t getType() const { return ((0x0000F00000000000LL&_guid)>>44); }
    inline void setType(u8_t type) { _guid &= ~0x0000F00000000000LL; _guid |= ((type&0xFLL)<<44); }

    inline GUID_t getCounter() const { return (0x00000FFFFFFFFFFFLL&_guid); }
    inline void setCounter(GUID_t counter) { _guid &= ~0x00000FFFFFFFFFFFLL; _guid |= (counter&0x00000FFFFFFFFFFFLL); }

    inline GUID_t getMaxCounter() const { return 0x00000FFFFFFFFFFFLL; }

    inline operator GUID_t() const { return _guid ;}

    inline GUID operator++(int) // i++
    {
        return GUID(_guid++);
    }

    inline GUID& operator++() // ++i
    {
        ++_guid;
        return *this;
    }

    inline bool isEmpty() const { return _guid == 0; }
    inline bool operator!() const { return isEmpty(); }
    inline bool operator<(const GUID& guid) const { return getRawValue() < guid.getRawValue(); }
    inline bool operator>=(const GUID& guid) const { return getRawValue() >= guid.getRawValue(); }

private:
    explicit GUID(u32_t);

private:
    GUID_t _guid;
};

class GUIDGenerator : public Singleton<GUIDGenerator>
{
public:
    GUIDGenerator()
    {
        MEMSET(&_nextGuid, 0x00, sizeof(_nextGuid));
    }

    // XXX: must be set before get
    inline void set(const GUID& min)
    {
        if (min.getType() >= GUID_MAX)
            return;
        _nextGuid[min.getType()] = min;
    }

    inline GUID get(GUIDType gt)
    {
        if (gt >= GUID_MAX)
            return GUID(GUID_t(0));
        if (_nextGuid[gt].getCounter() >= _nextGuid[gt].getMaxCounter() - 1)
        {
            // TODO: shutdown the server
            return GUID(GUID_t(0));
        }
        return _nextGuid[gt]++;
    }

    inline GUID getNextGuid(GUIDType gt) const { return _nextGuid[gt]; }

private:
    GUID _nextGuid[GUID_MAX];
};

#define gGUID GUIDGenerator::getInstance()
#define gGUIDGET(gt) gGUID.get(gt)

} // namespace buf

#endif // GUID_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

