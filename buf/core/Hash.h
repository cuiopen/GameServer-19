
#ifndef HASH_H_
#define HASH_H_

#ifndef _NDEBUG
#include "RTime.h"
#endif

#include "Config.h"

__BEGIN
#ifdef __CYGWIN32__
extern size_t strlen(const char*);
#else
#include <string.h>
#endif
__END

namespace buf
{

// seed should be 31 131 1313 13131 131313 etc..
unsigned int bkdr_hash(const char* str, size_t size, int seed = 31)
{
#ifndef _NDEBUG
    //TimeFilm t;
#endif
    if (!str || !seed || !size)
        return 0;

    char* pstr = const_cast<char*>(str);
    unsigned int hash = 0;

    while (size--)
        hash = hash * seed + (*pstr++);

    return hash; // hash & 0x7fffffff;
}

// From: http://www.azillionmonkeys.com/qed/hash.html
//#include "pstdint.h" /* Replace with <stdint.h> if appropriate */
#include <stdint.h>
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
      || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                               +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t super_fast_hash(const char* data, int len)
{
#ifndef _NDEBUG
    //TimeFilm t;
#endif
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || !data) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--)
    {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        /* data  += 2*sizeof (uint16_t); */
        /* data  += sizeof (uint16_t)+sizeof(uint16_t); */
        data  += sizeof (uint16_t)<<1;
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem)
    {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= data[sizeof (uint16_t)] << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += *data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

unsigned int hash(const char* str)
{
    if (!str)
        return 0;
    return bkdr_hash(str, strlen(str));
}

unsigned int sfhash(const char* str)
{
    if (!str)
        return 0;

    return super_fast_hash(str, strlen(str));
}

class Hash
{
public:

private:

};

} // namespace buf

#endif // HASH_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

