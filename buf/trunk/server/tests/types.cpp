
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/TypeTraits.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    printf("is const : %d\n", is_const<const int>::value);
    printf("is const : %d\n", is_const<int>::value);
    printf("is pointer: %d\n", is_pointer<const int*>::value);
    printf("is const : %d\n", is_const<const int*>::value);
    printf("is const : %d\n", is_const<int>::value);

    printf("is unsigned<unsigned> : %d\n", is_unsigned<unsigned>::value);
    printf("is unsigned<int>: %d\n", is_unsigned<int>::value);
    printf("is unsigned<char>: %d\n", is_unsigned<char>::value);
    printf("is unsigned<long>: %d\n", is_unsigned<long>::value);
    printf("is unsigned<unsigned long>: %d\n", is_unsigned<unsigned long>::value);

    typedef char* I;
    remove_pointer<I>::type i = 100;
    printf("sizeof(i):%lu = %d\n", sizeof(i), i);

    return 0;
}
