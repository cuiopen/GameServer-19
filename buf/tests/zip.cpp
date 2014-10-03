
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Zip.h"
#include "core/Zipper.h"
#include "core/Unzipper.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
#if 0
    //Zip zip(Zip::pointer("hello"),5);
    Zip zip;
    char buf[1024] = {0};
    SNPRINTF(buf, sizeof(buf), "hello world!OXOXOXOXOXOXOXOXOXOfuck");

    printf("before zip: %lu\n", sizeof(buf));
#if 0
    Vector<Zip::data_type> dst;
    Zip::size_type size = zip.zip(dst, (Zip::pointer)buf, sizeof(buf), Zip::LEVEL_BEST);
    //Zip::size_type size = zip.zip(dst, (Zip::pointer)buf, sizeof(buf));
    printf("after zip: %lu\n", size);

    Vector<Zip::data_type> undst;
    size = zip.unzip(undst, (Zip::pointer)&dst[0], dst.size());
    printf("after unzip: %lu, %s\n", size, &undst[0]);
#endif

#if 1
    Zip::pointer dst = 0;
    Zip::size_type size = zip.zip(&dst, (Zip::pointer)buf, sizeof(buf), Zip::LEVEL_BEST);
    printf("after zip: %lu\n", size);

    Zip::pointer undst = 0;
    size = zip.unzip(&undst, dst, size);
    printf("after unzip: %lu, %s\n", size, &undst[0]);

    zip.zipfree(dst);
    zip.zipfree(undst);
#endif
#endif

#if 1
    char buf[1024] = {0};
    SNPRINTF(buf, sizeof(buf), "hello world!OXOXOXOXOXOXOXOXOXOfuck");
    Zipper zipper(buf, sizeof(buf));

    printf("sizeof(Zipper): %lu\n", sizeof(Zipper));
    printf("Size of after zip: %lu\n", zipper.size());

    Unzipper unzipper(&zipper, zipper.size());
    printf("sizeof(Unzipper): %lu\n", sizeof(Unzipper));
    printf("After unzip(%lu): %s\n", unzipper.size(), &unzipper);

#endif


    return 0;
}
