
#ifndef ATOMIC_H_
#define ATOMIC_H_

#ifdef HAVE_SYS_ATOMIC_H
#   include <sys/atomic.h>
#   define HAVE_ATOMIC
#elif defined(HAVE_ASM_ATOMIC_H)
#   include "SpinLock.h"
#elif defined(__WIN32__)
#   define NOUSER
#   define NOGDI
#   include <windows.h>
#else
#   include "SpinLock.h"
#endif

namespace buf
{

class Atomic
{
public:
#ifdef HAVE_ATOMIC
    typedef atomic_t atomic_type;
#elif defined(__WIN32__)
    typedef long atomic_type;
#else
    typedef int atomic_type;
#endif

public:
    Atomic();
    Atomic(const int value);
#ifdef DEFINE_OPERATORS
    int operator+(const int change);
    int operator-(const int change);
    bool operator!(void);
#endif
    int operator=(const int value);
    int operator+=(const int change);
    int operator-=(const int change);
    int operator*=(const int change);
    int operator++(void);
    int operator++(int);
    int operator--(void);
    int operator--(int);
    operator int();

private:
    atomic_type atomic;
#ifdef HAVE_ATOMIC
#elif defined(__WIN32__)
#else
    FastLock mutex;
#endif
};

#ifdef HAVE_ATOMIC

inline Atomic::Atomic()
{
    atomic.counter = 0;
}

inline Atomic::Atomic(const int value)
{
    atomic.counter = value;
}

#ifdef DEFINE_OPERATORS

inline int Atomic::operator+(const int change)
{
    return atomic_read(&atomic) + change;
}

inline int Atomic::operator-(const int change)
{
    return atomic_read(&atomic) - change;
}

inline bool Atomic::operator!(void)
{
    return atomic_read(&atomic) ? false : true;
}

#endif // DEFINE_OPERATORS

inline int Atomic::operator=(const int value)
{
    atomic_set(&atomic, value);
    return atomic_read(&atomic);
}

inline int Atomic::operator+=(const int change)
{
    atomic_add(change, &atomic);
    return atomic_read(&atomic);
}

inline int Atomic::operator-=(const int change)
{
    atomic_sub(change, &atomic);
    return atomic_read(&atomic);
}

inline int Atomic::operator++(void)
{
    atomic_inc(&atomic);
    return atomic_read(&atomic);
}

inline int Atomic::operator++(int)
{
    int i = atomic_read(&atomic);
    atomic_inc(&atomic);
    return i;
}

inline int Atomic::operator--(void)
{
    atomic_dec(&atomic);
    return atomic_read(&atomic);
}

inline int Atomic::operator--(int)
{
    int i = atomic_read(&atomic);
    atomic_dec(&atomic);
    return i;
}

inline Atomic::operator int()
{
    return atomic_read(&atomic);
}

#elif defined(__WIN32__)

inline Atomic::Atomic()
{
    atomic = 0;
};

inline Atomic::Atomic(const int value)
{
    atomic = value;
};

#ifdef DEFINE_OPERATORS

inline int Atomic::operator+(const int change)
{
    return atomic + change;
};

inline int Atomic::operator-(const int change)
{
    return atomic - change;
};

inline bool Atomic::operator!(void)
{
    return atomic ? false : true;
};

#endif // DEFINE_OPERATORS

inline int Atomic::operator=(const int value)
{
    InterlockedExchange(&atomic, value);
    return value;
};

inline int Atomic::operator+=(const int change)
{
    InterlockedExchangeAdd(&atomic, change);
    return atomic;
}

inline int Atomic::operator-=(const int change)
{
    InterlockedExchangeAdd(&atomic, -change);
    return atomic;
}

inline int Atomic::operator++(void)
{
    return InterlockedIncrement(&atomic);
};

inline int Atomic::operator++(int)
{
    int i = atomic;
    InterlockedIncrement(&atomic);
    return i;
};

inline int Atomic::operator--(void)
{
    return InterlockedDecrement(&atomic);
};

inline int Atomic::operator--(int)
{
    int i = atomic;
    InterlockedDecrement(&atomic);
    return i;
};

inline Atomic::operator int()
{
    return atomic;
};

#else

inline Atomic::Atomic()
{
    atomic = 0;
}

inline Atomic::Atomic(const int value)
{
    atomic = value;
}

#ifdef DEFINE_OPERATORS

inline int Atomic::operator+(const int change)
{
    ScopeFastLock lock(mutex);
    return atomic + change;
}

inline int Atomic::operator-(const int change)
{
    ScopeFastLock lock(mutex);
    return atomic - change;
}

inline bool Atomic::operator!(void)
{
    ScopeFastLock lock(mutex);
    return atomic ? false : true;
}

#endif // DEFINE_OPERATORS

inline int Atomic::operator=(const int value)
{
    ScopeFastLock lock(mutex);
    atomic = value;
    return value;
}

inline int Atomic::operator+=(const int change)
{
    ScopeFastLock lock(mutex);
    atomic += change;
    return atomic;
}

inline int Atomic::operator-=(const int change)
{
    ScopeFastLock lock(mutex);
    atomic -= change;
    return atomic;
}

inline int Atomic::operator++(void)
{
    ScopeFastLock lock(mutex);
    return ++atomic;
}

inline int Atomic::operator++(int)
{
    ScopeFastLock lock(mutex);
    return atomic++;
}

inline int Atomic::operator--(void)
{
    ScopeFastLock lock(mutex);
    return --atomic;
}

inline int Atomic::operator--(int)
{
    ScopeFastLock lock(mutex);
    return atomic--;
}

inline Atomic::operator int()
{
    ScopeFastLock lock(mutex);
    return atomic;
}

#endif // !__WIN32__

} // namespace buf

#endif // ATOMIC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

