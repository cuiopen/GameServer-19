
#ifndef OBJECTPOOL_H_
#define OBJECTPOOL_H_

#include "SpinLock.h"
#include "Singleton.h"

namespace buf
{

#if 0
#define BIN_PER_BLK 256
#else
#define BIN_PER_BLK 3
#endif

#pragma pack(1)

struct ObjectPoolBlock // A
{
    ObjectPoolBlock* _next;
    char _mem[1];
};
#define OPBLKSZ (sizeof(ObjectPoolBlock)-sizeof(char))

struct ObjectPoolBin // B
{
    ObjectPoolBin* _next;
    int _free; // XXX: for align so use int
    char _mem[1];
};
#define OPBINSZ (sizeof(ObjectPoolBin)-sizeof(char))

#pragma pack()

template <typename T>
class ObjectPool : public Singleton<ObjectPool<T> >
{
public:
    ObjectPool() : _blks(0), _freeobjects(0), _bins(0), _freelist(0)
    {}

    ~ObjectPool()
    {
        printInfo();
        ObjectPoolBlock* blk = _bins;
        while (blk)
        {
            _bins = blk->_next;

            size_t binsize = sizeof(T);
            ObjectPoolBin* bin = reinterpret_cast<ObjectPoolBin*>(blk->_mem);
            for (int i = 0; i < BIN_PER_BLK && bin; ++i)
            {
                if (!bin->_free)
                {
                    T* ptr = reinterpret_cast<T*>(bin->_mem);
                    ptr->~T();
                }

                if (i != BIN_PER_BLK - 1)
                    bin = reinterpret_cast<ObjectPoolBin*>(reinterpret_cast<char*>(bin)+OPBINSZ+binsize);
            }

            delete [] reinterpret_cast<char*>(blk);
            blk = _bins;
        }
    }

    void* getObjectPlace()
    {
        ScopeSpinLock lck(_lck);

        if (_freelist)
        {
            --_freeobjects;
            ObjectPoolBin* bin = _freelist;
            _freelist = bin->_next;
            bin->_next = 0;
            bin->_free = false;
            return bin->_mem;
        }

        size_t binsize = sizeof(T);
        ObjectPoolBlock* blk = reinterpret_cast<ObjectPoolBlock*>(new char[(binsize+OPBINSZ)*BIN_PER_BLK+OPBLKSZ]);
        if (!blk)
            return 0;
        blk->_next = _bins;
        _bins = blk;
        ++_blks;

        ObjectPoolBin* bin = reinterpret_cast<ObjectPoolBin*>(blk->_mem);
        ObjectPoolBin* head = bin;
        ObjectPoolBin* next = 0;
        for (int i = 0; i < BIN_PER_BLK && bin; ++i)
        {
            ++_freeobjects;
            if (i == BIN_PER_BLK - 1)
                next = _freelist;
            else
                next = reinterpret_cast<ObjectPoolBin*>(reinterpret_cast<char*>(bin)+OPBINSZ+binsize);

            bin->_next = next;
            bin->_free = true;
            bin = next;
        }
        --_freeobjects;
        _freelist = head->_next;
        head->_next = 0;
        head->_free = false;

        return head->_mem;
    }

    T* get()
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T;
        return 0;
    }

    template <typename P1>
    T* get(P1& p1)
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T(p1);
        return 0;
    }

    template <typename P1, typename P2>
    T* get(P1& p1, P2& p2)
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T(p1, p2);
        return 0;
    }

    template <typename P1, typename P2, typename P3>
    T* get(P1& p1, P2& p2, P3& p3)
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T(p1, p2, p3);
        return 0;
    }

    template <typename P1, typename P2, typename P3, typename P4>
    T* get(P1& p1, P2& p2, P3& p3, P4& p4)
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T(p1, p2, p3, p4);
        return 0;
    }

    template <typename P1, typename P2, typename P3, typename P4, typename P5>
    T* get(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5)
    {
        void* place = getObjectPlace();
        if (place)
            return new (place) T(p1, p2, p3, p4, p5);
        return 0;
    }

    void put(T* ptr)
    {
        if (!ptr)
            return;
        ObjectPoolBin* bin = reinterpret_cast<ObjectPoolBin*>(reinterpret_cast<char*>(ptr)-OPBINSZ);
        if (!bin)
            return;
        if (bin && bin->_free)
            return;
        ptr->~T();

        ++_freeobjects;

        {
            ScopeSpinLock lck(_lck);
            bin->_next = _freelist;
            bin->_free = true;
            _freelist = bin;
        }
    }

    void printInfo()
    {
        fprintf(stderr, "blks: %d, freeobjects: %d\n", _blks, _freeobjects);
    }

private:
    int _blks;
    int _freeobjects;

    ObjectPoolBlock* _bins;
    ObjectPoolBin* _freelist;
    SpinLock _lck;
};

} // namespace buf

#endif // OBJECTPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

