
#include "Config.h"
#include "MemPool.h"

namespace buf
{

#if 0
#define BIN_MAX 4096
#define BIN_PER_BLK 64
#else
#define BIN_MAX 256
#define BIN_PER_BLK 2
#endif

MemPool::MemPool() : _blks(0), _freebins(0), _bigbins(0)
{
    MEMZRO(_freelist, sizeof(_freelist));
    MEMZRO(_bins, sizeof(_bins));
}

MemPool::~MemPool()
{
    _blks = 0;
    _freebins = 0;
    _bigbins = 0;

    for (int i = 0; i < MBP_BINS; ++i)
    {
        MemPoolBlock* blk = _bins[i];
        MemPoolBlock* yablk = blk;
        while (blk)
        {
            yablk = blk->_next;
            delete [] reinterpret_cast<char*>(blk);
            blk = yablk;
        }
    }
}

void MemPool::printInfo()
{
    fprintf(stderr, "blks: %d, freebins: %d, bigbins: %d\n", _blks, _freebins, _bigbins);
}

inline size_t getBinSize(size_t size, int& whichbin)
{
    size_t binsz = 1;
    int which = 0;

    if (size <= 1)
        return 1;

    while (size > binsz)
    {
        binsz <<= 1;
        ++which;
    }

    whichbin = which;
    return binsz;
}

void* MemPool::alloc(size_t size)
{
    DEBUG("allocing: %lu\n", size);
    ScopeSpinLock lck(_lck);
    if (size > BIN_MAX)
    {
        MemPoolBin* bin = reinterpret_cast<MemPoolBin*>(new char[size+MPBLKSZ]);
        if (!bin)
            return 0;
        bin->_next = 0;
        bin->_freelist = 0;
        ++_bigbins;
        DEBUG("alloc big(%lu): %p\n", size, bin->_mem);
        return bin->_mem;
    }

    int whichbin = 0;
    size_t binsize = getBinSize(size, whichbin);
    MemPoolBin* bin = _freelist[whichbin];
    if (bin)
    {
        _freelist[whichbin] = bin->_next;
        bin->_next = 0;
        bin->_freelist = &_freelist[whichbin];
        --_freebins;
        DEBUG("alloc(%lu): %p\n", size, bin->_mem);
        return bin->_mem;
    }

    MemPoolBlock* blk = reinterpret_cast<MemPoolBlock*>(new char[(binsize+MPBINSZ)*BIN_PER_BLK+MPBLKSZ]);
    if (!blk)
        return 0;
    ++_blks;

    blk->_next = _bins[whichbin];
    _bins[whichbin] = blk;

    bin = reinterpret_cast<MemPoolBin*>(blk->_mem);
    MemPoolBin* head = bin;
    MemPoolBin* next = 0;
    for (int i = 0; i < BIN_PER_BLK && bin; ++i)
    {
        if (i == BIN_PER_BLK - 1)
            next = _freelist[whichbin];
        else
            next = reinterpret_cast<MemPoolBin*>((reinterpret_cast<char*>(bin)+MPBINSZ+binsize));

        if (bin)
            DEBUG("freebin: %p\n", bin->_mem);

        bin->_next = next;
        bin->_freelist = &_freelist[whichbin];
        bin = next;
        ++_freebins;
    }
    _freelist[whichbin] = head->_next;
    head->_next = 0;
    --_freebins;

    DEBUG("alloc(%lu): %p\n", size, head->_mem);
    return head->_mem;
}

void MemPool::free(void* ptr)
{
    if (!ptr)
        return;
    ScopeSpinLock lck(_lck);
    MemPoolBin* bin = reinterpret_cast<MemPoolBin*>((reinterpret_cast<char*>(ptr)-MPBINSZ));
    if (bin->_freelist)
    {
        DEBUG("free: %p\n", bin->_mem);
        bin->_next = *(bin->_freelist);
        *(bin->_freelist) = bin;
        ++_freebins;
    }
    else
    {
        delete [] reinterpret_cast<char*>(bin);
        --_bigbins;
    }
}

}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

