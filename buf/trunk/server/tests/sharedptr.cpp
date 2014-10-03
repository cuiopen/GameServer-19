
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Config.h"
#include "core/SharedPtr.h"
using namespace buf;

class A
{
public:
    void hello(void)
    {
        printf("hello\n");
    }

    A()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }

    virtual ~A()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
};

class B : public A
{
public:
    B()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }

    ~B()
    {
        printf("%s\n", __PRETTY_FUNCTION__);
    }
};

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
#if 0
    SharedPtr<int> p = new int;
    int* pi = p.get();

    SharedPtr<A> pa = new A();
    pa->hello();

    SharedPtr<A> ppa = new A();

    if (pa != ppa)
        printf("not equl\n");

    SharedPtr<A> ypa = ppa;
    if (ypa == ppa)
        printf("equl\n");

    SharedPtr<A> pppp;
    if (pppp != ppa)
        printf("not equl\n");
#endif

#if 0
    SharedPtr<A> pa = BUFNEW A();
    SharedPtr<A> yapa = pa;
    SharedPtr<A> yapa1 = pa;
    yapa->hello();
#endif

#if 0
    SharedPtr<B> pb = BUFNEW B();
    SharedPtr<A> jpa = pb;
    SharedPtr<B> pbb;
    //pbb = jpa;
#endif

    SharedPtr<B> pb;
    SharedPtr<B> yapb = BUFNEW B();
    SharedPtr<A> pa = BUFNEW B();
    //pb = BUFNEW B();
    pb = yapb;
    pa = pb;

    //A* pa = BUFNEW A;
    //DELETE(pa);

	return 0;
}

