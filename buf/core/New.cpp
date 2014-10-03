
#include "New.h"
#include <new>
#include <cstddef>

#ifndef _NDEBUG

// XXX: 
// new : 只负责构造前的内存分配
// delete : 只负责析构后的内存回收

enum alloc_type
{
    AT_BUFNEW,
    AT_PLACEBUFNEW
};

struct allocinfo
{
    size_t _size;
    const char* _file;
    int _line;
    int _type;
};

#ifdef __GNUC__
#include <ext/hash_map> // XXX: warning
typedef __gnu_cxx::hash_map<ptrdiff_t, allocinfo> alloc_record;
#else // XXX:
#include <hash_map>
typedef hash_map<void*, allocinfo> alloc_record;
#endif // __GNUC__

allocinfo alloc;
unsigned long long g_total;
alloc_record g_allocs;

void* operator new(size_t, void* ptr, const char*, int) throw() 
{
    return ptr;
}

void* operator new[](size_t, void* ptr, const char*, int) throw()
{
    return ptr;
}

void* operator new(size_t size, const char* file, int line) throw()
{
    void* ptr = ::malloc(size);
    alloc._size = size;
    alloc._file = file;
    alloc._line = line;
    if (g_allocs.insert(std::make_pair(reinterpret_cast<ptrdiff_t>(ptr), alloc)).second)
        g_total += size;
    return ptr;
}

void* operator new[](size_t size, const char* file, int line) throw()
{
    return operator new(size, file, line);
}

void operator delete(void* ptr) throw()
{
    if (!ptr)
        return;
    alloc_record::iterator i = g_allocs.find(reinterpret_cast<ptrdiff_t>(ptr));
    if (i != g_allocs.end())
    {
        g_total -= i->second._size;
        g_allocs.erase(i);
    }
    ::free(ptr);
    return;
}

void operator delete(void* ptr, const char*, int) throw()
{
    operator delete(ptr);
}

void operator delete[](void* ptr, const char*, int) throw()
{
    operator delete(ptr);
}

void operator delete[](void* ptr, size_t, const char*, int) throw()
{
    operator delete(ptr);
}

void* operator new(size_t, const std::nothrow_t&) throw()
{
    return 0;
}
void* operator new[](size_t, const std::nothrow_t&) throw()
{
    return 0;
}

#ifdef __GNUC__
__attribute__((constructor))
void alloc_init()
{
    g_allocs.clear();
}

__attribute__((destructor))
void alloc_leak()
{
    fprintf(stdout, "MEMORY ALLOCATED SIZE: %llu\n", g_total);
    if (!g_allocs.size())
    {
        fprintf(stdout, "NO MEMORY LEAKS\n");
        return;
    }
    for (alloc_record::iterator i = g_allocs.begin(), e = g_allocs.end(); i != e; ++i)
    {
        fprintf(stdout, "LEAK: %s:%d %lu\n", i->second._file, i->second._line, i->second._size);
    }
}
#else
#error "no implementation"
#endif

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

