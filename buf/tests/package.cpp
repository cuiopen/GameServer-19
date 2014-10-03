
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "common/Package.h"
#include "common/Item.h"
using namespace buf;

class E: public Enumerator<Item<GameObject<> > >
{
    virtual bool operator()(Item<GameObject<> >* ptr)
    {
        printf("guid: %lu, name:%s\n", (GUID_t)ptr->getId(), ptr->getName());
        return true;
    }
};

void test1()
{
    Package<Item<GameObject<> > > p(100, 50);

    GameObject<>* ib = new GameObject<>(GUID(GUID_t(1)));
    Item<GameObject<> >* i = new Item<GameObject<> >(GUID(GUID_t(2)), ib);
    p.add(i);
    Item<GameObject<> >* ii = p.getItemById(GUID(GUID_t(2)));
    if (ii)
        printf("ii: %lu\n", (GUID_t)ii->getId());

    E e;
    p.enumerate(e);
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test1();
	return 0;
}

