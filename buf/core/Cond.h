
#ifndef COND_H_
#define COND_H_

#include "Config.h"
#include "Noncopyable.h"

#ifdef UNIXLIKE
#include "linux/Cond_POSIX.h"
#endif

#ifdef WIN
#endif

namespace buf
{
    
class Cond : public CondImpl, public Noncopyable
{
public:
    Cond() : CondImpl() {}
    ~Cond() {}

    inline bool wait(const Mutex& mutex, const struct timespec* abstime = 0)
    {
        return waitImpl(mutex, abstime);
    }

    inline void signal()
    {
        return signalImpl();
    }

    inline void broadcast()
    {
        return broadcastImpl();
    }
};

} // namespace buf

#endif // COND_H_

