
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Object.h"
#include "core/ObjectManager.h"
#include <algorithm>
using namespace buf;

class Item : public Object<IDGenerator>
{
public:
    Item(const key_type& id) : Object<IDGenerator>(id)
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
    ~Item()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }

    void hello()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
};

class ItemNT : public ObjectNT<>
{
public:
    ItemNT(const key_type& id, const std::string& name, const temp_type& tempid)
        : ObjectNT<>(id, name, tempid)
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
    ~ItemNT()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }

    void hello()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
};

void test(void)
{
    ObjectManager<> m;
    Object<>* obj = Create<Item>();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());
    obj = Create<Item>();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());
    obj = Create<Item>();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());

    obj = Create<Object<> >();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());
    obj = Create<Object<> >();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());
    obj = Create<Object<> >();
    m.add(obj->id(), obj);
    printf("%u\n", obj->id());

    m.del(2);


    ItemNT* itemn = CreateNT<ItemNT>("hello");
    printf("%u, %s, %u\n", itemn->id(), itemn->name().c_str(), itemn->tempid());
    while (!m.add(itemn->id(), itemn))
        itemn->id()++;
    printf("%u, %s, %u\n", itemn->id(), itemn->name().c_str(), itemn->tempid());

}

class V : public Enumerator<ItemNT>
{
public:
    bool operator()(ItemNT* i)
    {
        printf("%u, %s, %u\n", i->id(), i->name().c_str(), i->tempid());
        return true;
    }
};

void test1(void)
{
    ObjectManagerNT<ItemNT> m;
    ItemNT* item = CreateNT<ItemNT>("yangyoufa");
    m.add(item);
    ItemNT* yaitem = m.byName("yangyoufa");
    printf("%u, %s, %u\n", yaitem->id(), yaitem->name().c_str(), yaitem->tempid());
    item = CreateNT<ItemNT>("yangy");
    m.add(item);
    item = CreateNT<ItemNT>("yangyo");
    m.add(item);
    item = CreateNT<ItemNT>("yangyou");
    m.add(item);

#if 0
    yaitem = m.byName("yangyou");
    printf("%u, %s, %u\n", yaitem->id(), yaitem->name().c_str(), yaitem->tempid());
    yaitem = m.byId(2);
    printf("%u, %s, %u\n", yaitem->id(), yaitem->name().c_str(), yaitem->tempid());
#endif

    V v;
    //m.enumerate(v);
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test1();
	return 0;
}

