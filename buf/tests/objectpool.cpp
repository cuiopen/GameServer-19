
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/ObjectPool.h"
#include "core/RTime.h"
#include "core/Random.h"
#include "common/GameObject.h"
using namespace buf;

class SmallObject
{
public:
    SmallObject()
    {
        fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    }

    SmallObject(int a)
    {
        fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    }

    SmallObject(int a, const char* b)
    {
        fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    }

    ~SmallObject()
    {
        fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
    }


private:
    int a;
    int b;
    float c;
    double d;
};

void test1()
{
    ObjectPool<SmallObject> op;
    SmallObject* s = op.get();
    op.put(s);
    s = op.get();
    s = op.get();
    s = op.get();
    s = op.get();

    printf("++++++++++++++++\n");
}

void test2()
{
    TimeFilm tm("object pool: ");
    ObjectPool<SmallObject> op;
    SmallObject* s = 0;

    //int m = 1000000;
    int m = 100;
    Random rnd;
    const char* hello = "hello";
    while (--m)
    {
        //s = op.get();
        s = op.get(m,hello);
#if 1
        if (s && rnd(2))
        {
            printf("put\n");
            op.put(s);
        }
#endif
    }
}

int main(int argc, char* argv[])
{
    //test1();
    test2();
	return 0;
}

