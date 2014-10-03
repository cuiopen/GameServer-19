
#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include "SpinLock.h"
#include "Singleton.h"

namespace buf
{

#define MBP_BINS 13
#if MBP_BINS > 30
#   error "MBP_BINS must be less then 31"
#endif

/**
 * [A [B] [B] [B] [B]] [A [B] [B] ...] ...
 **/

#pragma pack(1)

struct MemPoolBlock // A
{
    MemPoolBlock* _next;
    char _mem[1];
};
#define MPBLKSZ (sizeof(MemPoolBlock)-sizeof(char))

struct MemPoolBin // B
{
    MemPoolBin* _next;
    MemPoolBin** _freelist;
    char _mem[1];
};
#define MPBINSZ (sizeof(MemPoolBin)-sizeof(char))

#pragma pack()

class MemPool : public Singleton<MemPool>
{
public:
    MemPool();
    ~MemPool();

    void* alloc(size_t size);
    void free(void* ptr);

    inline int getBlocks() const { return _blks; }
    inline int getFreeBins() const { return _freebins; }
    inline int getBigBins() const { return _bigbins; }

    void printInfo();

private:
    int _blks;
    int _freebins;
    int _bigbins;

    MemPoolBlock* _bins[MBP_BINS]; // 2^N
    MemPoolBin* _freelist[MBP_BINS];

    SpinLock _lck;
};

} // namespace buf

#endif // MEMPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

