
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/GameObjectMgr.h"
#include "common/GUID.h"
using namespace buf;

class E: public Enumerator<GameObject<GUID> >
{
    virtual bool operator()(GameObject<GUID>* ptr)
    {
        printf("guid: %lu, name:%s\n", (GUID_t)ptr->getId(), ptr->getName());
        return true;
    }
};

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    //GameObjectMgr<GOMgrById<GUID, GameObject<GUID>*>, GOMgrByMultiName<GameObject<GUID>*> > gom;
    GameObjectMgr<GOMgrById<GUID, GameObject<GUID>*>, GOMgrByName<GameObject<GUID>*> > gom;

    GameObject<GUID>* a = new GameObject<GUID>();
    a->setId(GUID(GUID_t(GUID_t(1))));
    a->setName("YYF");
    printf("push %lu,%s : %s [%lu]\n" , (u64_t)a->getId(), a->getName(), gom.push(a)?"true":"false", gom.size());;

    GameObject<GUID>* b = new GameObject<GUID>();
    b->setId(GUID(GUID_t(2)));
    b->setName("YYF");
    printf("push %lu,%s : %s [%lu]\n" , (u64_t)b->getId(), b->getName(), gom.push(b)?"true":"false", gom.size());;

    GameObject<GUID>* c = new GameObject<GUID>();
    c->setId(GUID(GUID_t(3)));
    c->setName("yangyoufa");
    printf("push %lu,%s : %s [%lu]\n" , (u64_t)c->getId(), c->getName(), gom.push(c)?"true":"false", gom.size());;

    E e;
    gom.enumerate(e);

    GameObject<GUID>* n = NULL;
    //gom.find("YYF", n, true);
    gom.find("YYF", n);
    if (n)
        printf("find %lu,%s\n", (u64_t)n->getId(), n->getName());
    n = 0;
    gom.find(GUID(GUID_t(1)), n);
    if (n)
        printf("find %lu,%s\n", (u64_t)n->getId(), n->getName());
    //printf("%d, %s\n", gom.find(a->getId()));

	return 0;
}
