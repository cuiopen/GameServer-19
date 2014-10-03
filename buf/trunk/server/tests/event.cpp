
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Event.h"
using namespace buf;

class Cb : public Event
{
public:
    Cb(int fd, short event) : Event(fd, event), a(1000)
    {
    }
    ~Cb() {}

    virtual bool onEvent()
    {
        printf("hello\n");
        return false;
    }

    int a;
};

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    EventManager em(100);
    em.addEvent(new Cb(1,1));
    em.delEvent(2);
    //em.delEvent(1);
    em.modEvent(new Cb(1,1));
    em.delEvent(1);

    std::vector<Cb*> evs;
    em.wait(evs);

	return 0;
}

