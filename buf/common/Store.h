
#ifndef STORE_H_
#define STORE_H_

#include "core/Config.h"
#include "ItemType.h"
#include <vector>

namespace buf
{

enum
{
    // 1 - add 2 - del 3 - clear
    STORE_ADD = 1,
    STORE_DEL = 2,
    STORE_CLR = 3
};

template <typename ID>
struct StoreItem
{
    ID id;
    int orgprice;   // 原价
    int price;      // 现价
    u32_t begin;    // 折扣开始时间，如果原价=现价不起作用
    u32_t end;      // 折扣结束时间，同上
    bool insell;    // 是否上架

    const StoreItem& operator=(const StoreItem& si)
    {
        init(si);
        return *this;
    }

    StoreItem& operator=(StoreItem& si)
    {
        init(si);
        return *this;
    }

    void reset()
    {
        orgprice = 0;
        price = 0;
        begin = 0;
        end = 0;
        insell = false;
    }

    void init(const StoreItem& si)
    {
        id = si.id;
        orgprice = si.orgprice;
        price = si.price;
        begin = si.begin;
        end = si.end;
        insell = si.insell;
    }
};

template <typename ID, int STORE_MAX = 10>
class Store
{
public:
    typedef std::vector<StoreItem<ID> > container;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

public:
    Store() {}
    virtual ~Store()
    {
        clear();
    }

    virtual bool init()
    {
        return false;
    }

    virtual bool reinit()
    {
        return false;
    }

    void clear()
    {
        for (int i = 0; i < STORE_MAX; ++i)
            _items[i].clear();
        doNotify(STORE_CLR);
    }

    const StoreItem<ID>* find(int type, const ID& id) const
    {
        if (type >= STORE_MAX)
            return 0;

        for (const_iterator i = _items[type].begin(); i != _items[type].end(); ++i)
        {
            if (i->id == id)
                return &*i;
        }

        return 0;
    }

    bool add(int type, const StoreItem<ID>& si, bool pre = false, bool notify = false)
    {
        if (type >= STORE_MAX)
            return false;
        StoreItem<ID>* s = const_cast<StoreItem<ID>*>(find(type, si.id));
        if (s)
        {
            *s = si;
        }
        else
        {
            if (pre)
                _items[type].insert(_items[type].begin(), si);
            else
                _items[type].push_back(si);
        }

        if (notify)
            doNotify(STORE_ADD, &si);

        return true;
    }

    bool remove(int type, const ID& id)
    {
        if (type >= STORE_MAX)
            return 0;

        for (iterator i = _items[type].begin(); i != _items[type].end(); ++i)
        {
            if (i->id == id)
            {
                doNotify(STORE_DEL, &*i);
                _items[type].erase(i);
                return true;
            }
        }
        return false;
    }

    virtual void doNotify(int /*type  = 1 - add 2 - del 3 - clear */, const StoreItem<ID>* /*si */= NULL) {}
    virtual void sendAll(int /*type*/) {}

private:
    container _items[STORE_MAX];
};

} // namespace buf

#endif // STORE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

