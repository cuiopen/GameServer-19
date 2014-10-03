
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/DataManager.h"
#include "common/GameObject.h"
#include "core/Enumerator.h"
using namespace buf;

struct BIN
{
    char id;
    char a;
    char b;
};

struct MEM : public GameObjectI<u32_t>
{
    bool init(const BIN* b)
    {
        setId(b->id);
        return true;
    }
};

class E: public Enumerator<MEM>
{
    virtual bool operator()(MEM* ptr)
    {
        printf("id: %u\n", ptr->getId());
        return true;
    }
};

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    //DataManager<MEM, BIN, GameObjectMgr<GOMgrById<typename MEM::id_type, MEM*> > > dm;
    DataManager<MEM, BIN, GameObjectMgr<GOMgrById<MEM::id_type, MEM*> > > dm;
    dm.loadFromFile("/tmp/t");
    const MEM* m = dm.getById(97);
    //printf("id: %d, name: %s\n", m->getId(), m->getName());
    printf("id: %d\n", m->getId());

    BIN bin;
    dm.loadFromDB(&bin);

	return 0;
}

