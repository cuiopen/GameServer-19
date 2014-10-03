
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <core/MinHeap.h>
using namespace buf;

struct G
{
    inline bool operator()(const int& a, const int& b) const
    {
        return a > b;
    }
};

int main(int argc, char* argv[])
{
    MinHeap<int, G> mh;
#if 0
    mh.push(1);
    mh.push(4);
    mh.push(2);
    mh.push(8);
    mh.push(5);
    mh.push(7);
#else
    mh.push(7);
    mh.push(1);
    mh.push(2);
    mh.push(2);
    mh.push(4);
    mh.push(5);
    mh.push(5);
    mh.push(8);
    mh.push(18);
    mh.push(100);
    mh.push(3);
    mh.push(0);
    mh.push(99);
    mh.push(1000);
#endif

#if 1
    unsigned size = mh.size();
    for (unsigned i = 0; i < size; ++i)
    {
        MinHeap<int, G>::pointer v = mh.pop();
        if (v)
        {
            printf("%u\n", v->val);
            DELETE(v);
        }
    }
#else
    mh.print();
#endif

	return 0;
}
