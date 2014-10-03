
#ifndef LOCK_H_
#define LOCK_H_

#include "Config.h"

__BEGIN

// spinlock

#define VOLATILE volatile 

#ifdef CPU_64BIT
typedef VOLATILE long __spinlock_t;
#define SPINLOCK_INITIALIZER 0L
#else
typedef VOLATILE long long __spinlock_t;
#define SPINLOCK_INITIALIZER 0
#endif

inline void __spinlock_init(__spinlock_t* lock)
{
    if (lock)
        *lock = SPINLOCK_INITIALIZER;
}

inline bool __spinlock_locked(__spinlock_t* lock)
{
    return *lock != 0;
}

inline bool __spinlock_try_lock(__spinlock_t* lock)
{
#if defined(__GNUC__) && (defined(__amd64__) || defined(__x86_64__))
    register int __locked;
    __asm__ __volatile ("xchgl %0, %1"
            : "=&r" (__locked), "=m" (*lock) : "0" (1)
            : "memory");
    return !__locked;
#else
    if (!*lock)
    {
        *lock = 1;
        return true;
    }
#endif // __GNUC__
    return false;
}

inline void __spinlock_unlock(__spinlock_t* lock)
{
#if defined(__GNUC__) && (defined(__amd64__) || defined(__x86_64__))
    register int __unlocked;
    __asm__ __volatile ("xchgl %0, %1"
            : "=&r" (__unlocked), "=m" (*lock) : "0" (0)
            : "memory");
#else
    if (*lock)
        *lock = 0;
#endif
    return ;
}

__END

#endif // SPINLOCK_H_

/* vim: set si ai nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

