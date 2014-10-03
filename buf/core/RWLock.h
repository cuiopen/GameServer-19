
#ifndef RWLOCK_Y_
#define RWLOCK_Y_ 

#include "Config.h"
#include "Noncopyable.h"

#ifdef UNIXLIKE
#include "linux/RWLock_POSIX.h"
#endif

namespace buf
{

class RWLock : public RWLockImpl, public Noncopyable
{
public:
    RWLock() {}
    ~RWLock() {}

    inline void rdlock() { rdlockImpl(); }
    inline void wrlock() { wrlockImpl(); }
    inline void unlock() { unlockImpl(); }
};

class ScopeRDLock : public Noncopyable
{
public:
    ScopeRDLock(RWLock& lock) : _lock(lock) { _lock.rdlock(); }
    ~ScopeRDLock() { _lock.unlock(); }

private:
    RWLock& _lock;
};

class ScopeRWLock : public Noncopyable
{
public:
    ScopeRWLock(RWLock& lock) : _lock(lock) { _lock.wrlock(); }
    ~ScopeRWLock() { _lock.unlock(); }

private:
    RWLock& _lock;
};

} // namespace buf

#endif // RWLOCK_Y_ 

