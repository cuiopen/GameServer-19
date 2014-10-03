
#ifndef PACKAGEITEM_H_
#define PACKAGEITEM_H_

namespace buf
{

enum pkgret
{
    PKGRET_FALSE = 0,
    PKGRET_TRUE,
    PKGRET_NOITEM,
    PKGRET_NOPOS,
    PKGRET_CANTMERGE,
    PKGRET_FULL,
    PKGRET_ERR
};

typedef u8_t pkgret_t;
typedef u8_t pkgtype_t;
typedef u16_t pkgsize_t;
typedef u8_t postype_t;
typedef u16_t itemtype_t;
typedef u16_t itemnum_t;

struct ItemPos
{
    ItemPos(pkgtype_t t = -1, pkgsize_t p = -1, postype_t pt = -1)
        : pkgtype(t), pos(p), postype(pt) {}

    pkgtype_t pkgtype;
    pkgsize_t pos;
    postype_t postype;
};

template <typename T, typename K>
class PackageItem
{
public:
    PackageItem() {}
    virtual ~PackageItem() {}

    virtual K getId() const { return 0; }
    virtual itemnum_t getNumber() const { return 0; }
    virtual void fixNumber(itemnum_t, int/*>0 plus, <0 sub*/) const { return; }

    virtual bool canMerge(const PackageItem*) const { return false; }
    virtual bool canSplit(itemnum_t) const { return false; }
    virtual bool canRemove(itemnum_t) const { return false; }
    virtual bool isBind() const { return false; }

    virtual void add(pkgtype_t, itemnum_t) { return; }
    virtual itemnum_t remove(itemnum_t) { return 0; }
    virtual void move(pkgtype_t, itemnum_t) { return; }
    virtual const PackageItem* merge(const PackageItem*) { return 0; }
    virtual const PackageItem* split(itemnum_t) { return 0; }

    virtual pkgsize_t getPos() const { return -1; }
    virtual pkgtype_t getPkgType() const { return -1; }
    virtual itemtype_t getType() const { return -1; }
};

} // namespace buf

#endif // PACKAGEITEM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

