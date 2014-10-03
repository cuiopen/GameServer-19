
#ifndef ASSERTION_H_
#define ASSERTION_H_

#include "Config.h"

__BEGIN

#include <stdlib.h> // for abort

#ifndef _NDEBUG

#define CRASH()\
do { \
    fprintf(stderr, "!!!Crashed!!!\n"); \
    abort(); \
} while(0);

#define ASSERT(assertion)\
do { \
    if (!(assertion))\
    { \
        fprintf(stderr, "%s:%d %s => %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #assertion); \
        CRASH(); \
    } \
} while(0);

#else // !_NDEBUG

#define CRASH() (void)0;
#define ASSERT(assertion) (void)0;

#endif // _NDEBUG

__END

#endif // ASSERTION_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

