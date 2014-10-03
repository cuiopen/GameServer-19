
#ifndef ITEM_H_
#define ITEM_H_

#include "GameObject.h"
#include "GUID.h"

namespace buf
{

// XXX:
//  IB must be drivered from GameObject<...>

template <typename IB, typename ID = GUID>
class Item : public GameObject<ID>
{
public:
    Item(const ID& id, IB* ib) : GameObject<ID>(id, ib?ib->getType():0, ib?ib->getName():NULL), _itemBase(ib) {}
    virtual ~Item() {}

    virtual bool isNormal() const { return false; }
    virtual bool isEquipment() const { return false; }

    // XXX: must be returned
    inline const IB& getItemBase() const { return *_itemBase; }

private:
    IB* _itemBase;
};

} // namespace buf

#endif // ITEM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

